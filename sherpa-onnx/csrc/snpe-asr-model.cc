#include "sherpa-onnx/csrc/snpe-asr-model.h"
#include <algorithm>
#include <memory>
#include <utility>
#include <numeric>
#include <array>
#include "string.h"
#include "sherpa-onnx/csrc/snpe_inc/hpp/inference.h"
#include "sherpa-onnx/csrc/online-transducer-decoder.h"
#include "sherpa-onnx/csrc/onnx-utils.h"
#include "sherpa-onnx/csrc/session.h"
#include "sherpa-onnx/csrc/cat.h"
#include "sherpa-onnx/csrc/macros.h"
#include "sherpa-onnx/csrc/onnx-utils.h"
#include "sherpa-onnx/csrc/text-utils.h"
#include "sherpa-onnx/csrc/unbind.h"

namespace sherpa_onnx {


std::unique_ptr<zdl::SNPE::SNPE> snpe_encoder;
static zdl::DlSystem::Runtime_t runtime = zdl::DlSystem::Runtime_t::DSP;
static zdl::DlSystem::RuntimeList runtimeList;
std::string dlc_path;
bool useUserSuppliedBuffers = true;
bool useIntBuffer = false;

SNPEAsrModel::SNPEAsrModel(
    AAssetManager *mgr, const OnlineModelConfig &config)
    : env_(ORT_LOGGING_LEVEL_WARNING),
      config_(config),
      sess_opts_(GetSessionOptions(config)),
      allocator_{} {
  {
    InitEncoder(mgr, config.transducer.encoder);
  }

  {
    auto buf = ReadFile(mgr, config.transducer.decoder);
    InitDecoder(buf.data(), buf.size());
  }

  {
    auto buf = ReadFile(mgr, config.transducer.joiner);
    InitJoiner(buf.data(), buf.size());
  }
}

void SNPEAsrModel::InitEncoder(AAssetManager *mgr, std::string encoder_name) {

    AAsset* asset_encoder = AAssetManager_open(mgr, encoder_name.c_str(), AASSET_MODE_UNKNOWN);
    long dlc_size_encoder = AAsset_getLength(asset_encoder);
    char* dlc_buffer_encoder = (char*) malloc(sizeof(char) * dlc_size_encoder);
    AAsset_read(asset_encoder, dlc_buffer_encoder, dlc_size_encoder);   

    bool usingInitCaching = true;
    std::unique_ptr<zdl::DlContainer::IDlContainer> container;

    //loading the model from buffer
    container = loadContainerFromBuffer(reinterpret_cast<const uint8_t *>(dlc_buffer_encoder), dlc_size_encoder);
    if (container == nullptr) {
        LOGE("Error while opening the container file.");
        return;
    }
    if(runtime != zdl::DlSystem::Runtime_t::UNSET)
    {
        bool ret = runtimeList.add(checkRuntime(runtime));
        if(ret == false){
            LOGE("Cannot set runtime");
            return;
        }
    } else{
        LOGE("Cannot set runtime");
        return;
    }

    runtimeList.clear();


    // Build for DSP runtime
    // zdl::DlSystem::Runtime_t runtime_dsp = zdl::DlSystem::Runtime_t::DSP;
    // zdl::DlSystem::Runtime_t runtime_cpu = zdl::DlSystem::Runtime_t::CPU;
    // runtimeList.add(runtime_dsp);
    // runtimeList.add(runtime_cpu);
    LOGI("BEFORE DSP BUILD ***********************");
    snpe_encoder = setBuilderOptions(container, runtime, runtimeList, useUserSuppliedBuffers, usingInitCaching);

    if (snpe_encoder == nullptr) {
        LOGE("SNPE Prepare failed: Builder option failed for DSP runtime");
        return;

    }

    if (usingInitCaching) {
        if (container->save(dlc_path)) {
            LOGI("Saved container into archive successfully");
        } else LOGE("Failed to save container into archive");
    }

}


void SNPEAsrModel::InitDecoder(void *model_data,
                                                  size_t model_data_length) {
  decoder_sess_ = std::make_unique<Ort::Session>(env_, model_data,
                                                 model_data_length, sess_opts_);

  GetInputNames(decoder_sess_.get(), &decoder_input_names_,
                &decoder_input_names_ptr_);

  GetOutputNames(decoder_sess_.get(), &decoder_output_names_,
                 &decoder_output_names_ptr_);

  // get meta data
  Ort::ModelMetadata meta_data = decoder_sess_->GetModelMetadata();
  if (config_.debug) {
    std::ostringstream os;
    os << "---decoder---\n";
    PrintModelMetadata(os, meta_data);
    SHERPA_ONNX_LOGE("%s", os.str().c_str());
  }

  Ort::AllocatorWithDefaultOptions allocator;  // used in the macro below
  SHERPA_ONNX_READ_META_DATA(vocab_size_, "vocab_size");
  SHERPA_ONNX_READ_META_DATA(context_size_, "context_size");
}

void SNPEAsrModel::InitJoiner(void *model_data,
                                                 size_t model_data_length) {
  joiner_sess_ = std::make_unique<Ort::Session>(env_, model_data,
                                                model_data_length, sess_opts_);

  GetInputNames(joiner_sess_.get(), &joiner_input_names_,
                &joiner_input_names_ptr_);

  GetOutputNames(joiner_sess_.get(), &joiner_output_names_,
                 &joiner_output_names_ptr_);

  // get meta data
  Ort::ModelMetadata meta_data = joiner_sess_->GetModelMetadata();
  if (config_.debug) {
    std::ostringstream os;
    os << "---joiner---\n";
    PrintModelMetadata(os, meta_data);
    SHERPA_ONNX_LOGE("%s", os.str().c_str());
  }
}


std::vector<Ort::Value> SNPEAsrModel::StackStates(
    const std::vector<std::vector<Ort::Value>> &states) const {
  int32_t batch_size = static_cast<int32_t>(states.size());
  int32_t num_encoders = static_cast<int32_t>(num_encoder_layers_.size());

  std::vector<const Ort::Value *> buf(batch_size);

  std::vector<Ort::Value> ans;
  ans.reserve(states[0].size());

  // cached_len
  for (int32_t i = 0; i != num_encoders; ++i) {
    for (int32_t n = 0; n != batch_size; ++n) {
      buf[n] = &states[n][i];
    }
    auto v = Cat(allocator_, buf, 1);  // (num_layers, 1)
    ans.push_back(std::move(v));
  }

  // cached_avg
  for (int32_t i = 0; i != num_encoders; ++i) {
    for (int32_t n = 0; n != batch_size; ++n) {
      buf[n] = &states[n][num_encoders + i];
    }
    auto v = Cat(allocator_, buf, 1);  // (num_layers, 1, encoder_dims)
    ans.push_back(std::move(v));
  }

  // cached_key
  for (int32_t i = 0; i != num_encoders; ++i) {
    for (int32_t n = 0; n != batch_size; ++n) {
      buf[n] = &states[n][num_encoders * 2 + i];
    }
    // (num_layers, left_context_len, 1, attention_dims)
    auto v = Cat(allocator_, buf, 2);
    ans.push_back(std::move(v));
  }

  // cached_val
  for (int32_t i = 0; i != num_encoders; ++i) {
    for (int32_t n = 0; n != batch_size; ++n) {
      buf[n] = &states[n][num_encoders * 3 + i];
    }
    // (num_layers, left_context_len, 1, attention_dims/2)
    auto v = Cat(allocator_, buf, 2);
    ans.push_back(std::move(v));
  }

  // cached_val2
  for (int32_t i = 0; i != num_encoders; ++i) {
    for (int32_t n = 0; n != batch_size; ++n) {
      buf[n] = &states[n][num_encoders * 4 + i];
    }
    // (num_layers, left_context_len, 1, attention_dims/2)
    auto v = Cat(allocator_, buf, 2);
    ans.push_back(std::move(v));
  }

  // cached_conv1
  for (int32_t i = 0; i != num_encoders; ++i) {
    for (int32_t n = 0; n != batch_size; ++n) {
      buf[n] = &states[n][num_encoders * 5 + i];
    }
    // (num_layers, 1, encoder_dims, cnn_module_kernels-1)
    auto v = Cat(allocator_, buf, 1);
    ans.push_back(std::move(v));
  }

  // cached_conv2
  for (int32_t i = 0; i != num_encoders; ++i) {
    for (int32_t n = 0; n != batch_size; ++n) {
      buf[n] = &states[n][num_encoders * 6 + i];
    }
    // (num_layers, 1, encoder_dims, cnn_module_kernels-1)
    auto v = Cat(allocator_, buf, 1);
    ans.push_back(std::move(v));
  }

  return ans;
}

std::vector<std::vector<Ort::Value>>
SNPEAsrModel::UnStackStates(
    const std::vector<Ort::Value> &states) const {
  assert(states.size() == num_encoder_layers_.size() * 7);
  int32_t batch_size = states[0].GetTensorTypeAndShapeInfo().GetShape()[1];
  int32_t num_encoders = num_encoder_layers_.size();

  std::vector<std::vector<Ort::Value>> ans;
  ans.resize(batch_size);

  // cached_len
  for (int32_t i = 0; i != num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 1);
    assert(v.size() == batch_size);

    for (int32_t n = 0; n != batch_size; ++n) {
      ans[n].push_back(std::move(v[n]));
    }
  }

  // cached_avg
  for (int32_t i = num_encoders; i != 2 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 1);
    assert(v.size() == batch_size);

    for (int32_t n = 0; n != batch_size; ++n) {
      ans[n].push_back(std::move(v[n]));
    }
  }

  // cached_key
  for (int32_t i = 2 * num_encoders; i != 3 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 2);
    assert(v.size() == batch_size);

    for (int32_t n = 0; n != batch_size; ++n) {
      ans[n].push_back(std::move(v[n]));
    }
  }

  // cached_val
  for (int32_t i = 3 * num_encoders; i != 4 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 2);
    assert(v.size() == batch_size);

    for (int32_t n = 0; n != batch_size; ++n) {
      ans[n].push_back(std::move(v[n]));
    }
  }

  // cached_val2
  for (int32_t i = 4 * num_encoders; i != 5 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 2);
    assert(v.size() == batch_size);

    for (int32_t n = 0; n != batch_size; ++n) {
      ans[n].push_back(std::move(v[n]));
    }
  }

  // cached_conv1
  for (int32_t i = 5 * num_encoders; i != 6 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 1);
    assert(v.size() == batch_size);

    for (int32_t n = 0; n != batch_size; ++n) {
      ans[n].push_back(std::move(v[n]));
    }
  }

  // cached_conv2
  for (int32_t i = 6 * num_encoders; i != 7 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 1);
    assert(v.size() == batch_size);

    for (int32_t n = 0; n != batch_size; ++n) {
      ans[n].push_back(std::move(v[n]));
    }
  }

  return ans;
}

std::vector<Ort::Value> SNPEAsrModel::GetEncoderInitStates() {
  // Please see
  // https://github.com/k2-fsa/icefall/blob/master/egs/librispeech/ASR/pruned_transducer_stateless7_streaming/zipformer.py#L673
  // for details

  int32_t n = static_cast<int32_t>(encoder_dims_.size());
  std::vector<Ort::Value> cached_len_vec;
  std::vector<Ort::Value> cached_avg_vec;
  std::vector<Ort::Value> cached_key_vec;
  std::vector<Ort::Value> cached_val_vec;
  std::vector<Ort::Value> cached_val2_vec;
  std::vector<Ort::Value> cached_conv1_vec;
  std::vector<Ort::Value> cached_conv2_vec;

  cached_len_vec.reserve(n);
  cached_avg_vec.reserve(n);
  cached_key_vec.reserve(n);
  cached_val_vec.reserve(n);
  cached_val2_vec.reserve(n);
  cached_conv1_vec.reserve(n);
  cached_conv2_vec.reserve(n);

  for (int32_t i = 0; i != n; ++i) {
    {
      std::array<int64_t, 2> s{num_encoder_layers_[i], 1};
      auto v =
          Ort::Value::CreateTensor<int32_t>(allocator_, s.data(), s.size());
      Fill<int32_t>(&v, 0);
      cached_len_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 3> s{num_encoder_layers_[i], 1, encoder_dims_[i]};
      auto v = Ort::Value::CreateTensor<float>(allocator_, s.data(), s.size());
      Fill(&v, 0);
      cached_avg_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], left_context_len_[i], 1,
                               attention_dims_[i]};
      auto v = Ort::Value::CreateTensor<float>(allocator_, s.data(), s.size());
      Fill(&v, 0);
      cached_key_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], left_context_len_[i], 1,
                               attention_dims_[i] / 2};
      auto v = Ort::Value::CreateTensor<float>(allocator_, s.data(), s.size());
      Fill(&v, 0);
      cached_val_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], left_context_len_[i], 1,
                               attention_dims_[i] / 2};
      auto v = Ort::Value::CreateTensor<float>(allocator_, s.data(), s.size());
      Fill(&v, 0);
      cached_val2_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], 1, encoder_dims_[i],
                               cnn_module_kernels_[i] - 1};
      auto v = Ort::Value::CreateTensor<float>(allocator_, s.data(), s.size());
      Fill(&v, 0);
      cached_conv1_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], 1, encoder_dims_[i],
                               cnn_module_kernels_[i] - 1};
      auto v = Ort::Value::CreateTensor<float>(allocator_, s.data(), s.size());
      Fill(&v, 0);
      cached_conv2_vec.push_back(std::move(v));
    }
  }

  std::vector<Ort::Value> ans;
  ans.reserve(n * 7);

  for (auto &v : cached_len_vec) ans.push_back(std::move(v));
  for (auto &v : cached_avg_vec) ans.push_back(std::move(v));
  for (auto &v : cached_key_vec) ans.push_back(std::move(v));
  for (auto &v : cached_val_vec) ans.push_back(std::move(v));
  for (auto &v : cached_val2_vec) ans.push_back(std::move(v));
  for (auto &v : cached_conv1_vec) ans.push_back(std::move(v));
  for (auto &v : cached_conv2_vec) ans.push_back(std::move(v));

  return ans;
}


std::vector<float> ConvertOrtValueToVectorFloat(Ort::Value& ortValue) {
    // Check if OrtValue is a tensor
    if (!ortValue.IsTensor()) {
        throw std::runtime_error("Provided Ort::Value is not a tensor.");
    }

    // Get the number of elements in the tensor
    size_t elementCount = ortValue.GetTensorTypeAndShapeInfo().GetElementCount();

    // Retrieve mutable pointer to tensor data
    float *tensorData = ortValue.GetTensorMutableData<float>();

    // Create a vector and copy data from tensor
    std::vector<float> result(tensorData, tensorData + elementCount);

    return result;
}

std::vector<float> ConvertOrtValuesToVectorFloat(const std::vector<Ort::Value>& ortValues) {
    std::vector<float> result;

    for (const auto& ortValue : ortValues) {
        // Check if OrtValue is a tensor
        if (!ortValue.IsTensor()) {
            throw std::runtime_error("One of the Ort::Value objects is not a tensor.");
        }

        // Get the number of elements in the tensor
        size_t elementCount = ortValue.GetTensorTypeAndShapeInfo().GetElementCount();

        // Retrieve mutable pointer to tensor data
        const float* tensorData = ortValue.GetTensorData<float>();

        // Append data to result vector
        result.insert(result.end(), tensorData, tensorData + elementCount);
    }

    return result;
}

std::vector<std::vector<float>> SNPEAsrModel::FormatInputStates(const std::vector<Ort::Value> &states) {
  int32_t batch_size = states[0].GetTensorTypeAndShapeInfo().GetShape()[1];
  int32_t num_encoders = num_encoder_layers_.size();

  std::vector<std::vector<float>> ans;
  ans.reserve(num_encoders * 7);

  // cached_len
  for (int32_t i = 0; i != num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 1);
    assert(v.size() == batch_size);
    auto vf = ConvertOrtValueToVectorFloat(v[0]);
    ans.push_back(std::move(vf));
  }

  // cached_avg
  for (int32_t i = num_encoders; i != 2 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 1);
    assert(v.size() == batch_size);
    auto vf = ConvertOrtValueToVectorFloat(v[0]);
    ans.push_back(std::move(vf));
  }

  // cached_key
  for (int32_t i = 2 * num_encoders; i != 3 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 2);
    assert(v.size() == batch_size);
    auto vf = ConvertOrtValueToVectorFloat(v[0]);
    ans.push_back(std::move(vf));
  }

  // cached_val
  for (int32_t i = 3 * num_encoders; i != 4 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 2);
    assert(v.size() == batch_size);
    auto vf = ConvertOrtValueToVectorFloat(v[0]);
    ans.push_back(std::move(vf));
  }

  // cached_val2
  for (int32_t i = 4 * num_encoders; i != 5 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 2);
    assert(v.size() == batch_size);
    auto vf = ConvertOrtValueToVectorFloat(v[0]);
    ans.push_back(std::move(vf));
  }

  // cached_conv1
  for (int32_t i = 5 * num_encoders; i != 6 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 1);
    assert(v.size() == batch_size);
    auto vf = ConvertOrtValueToVectorFloat(v[0]);
    ans.push_back(std::move(vf));
  }

  // cached_conv2
  for (int32_t i = 6 * num_encoders; i != 7 * num_encoders; ++i) {
    auto v = Unbind(allocator_, &states[i], 1);
    assert(v.size() == batch_size);
    auto vf = ConvertOrtValueToVectorFloat(v[0]);
    ans.push_back(std::move(vf));
  }

  // LOGI("states 0 size: %zu", ans[0].size());

  return ans;
}


Ort::Value SNPEAsrModel::ConvertVectorToOrtValue(const std::vector<float>& inputVector) {
    auto memory_info =
        Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeDefault);

    std::array<int64_t, 3> x_shape{1, 32, 512};

    Ort::Value x = Ort::Value::CreateTensor(memory_info, const_cast<float*>(inputVector.data()),
                                            inputVector.size(), x_shape.data(),
                                            x_shape.size());

    return x;
}

std::vector<Ort::Value> SNPEAsrModel::FormatEncoderOutputStates(std::vector<std::vector<float>> innerVector) {
  int32_t n = static_cast<int32_t>(encoder_dims_.size());
  std::vector<Ort::Value> cached_len_vec;
  std::vector<Ort::Value> cached_avg_vec;
  std::vector<Ort::Value> cached_key_vec;
  std::vector<Ort::Value> cached_val_vec;
  std::vector<Ort::Value> cached_val2_vec;
  std::vector<Ort::Value> cached_conv1_vec;
  std::vector<Ort::Value> cached_conv2_vec;

  cached_len_vec.reserve(n);
  cached_avg_vec.reserve(n);
  cached_key_vec.reserve(n);
  cached_val_vec.reserve(n);
  cached_val2_vec.reserve(n);
  cached_conv1_vec.reserve(n);
  cached_conv2_vec.reserve(n);

   Ort::MemoryInfo memInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

  for (int32_t i = 0; i != n; ++i) {
    {

      std::array<int64_t, 2> s{num_encoder_layers_[i], 1};
      Ort::Value v = Ort::Value::CreateTensor<float>(memInfo,
                                                      const_cast<float*>(innerVector[i].data()), 
                                                      innerVector[i].size(), 
                                                      s.data(), 
                                                      s.size());
      cached_len_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 3> s{num_encoder_layers_[i], 1, encoder_dims_[i]};
      Ort::Value v = Ort::Value::CreateTensor<float>(memInfo,
                                                      innerVector[i+5].data(), 
                                                      innerVector[i+5].size(), 
                                                      s.data(), 
                                                      s.size());
      cached_avg_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], left_context_len_[i], 1,
                               attention_dims_[i]};
      Ort::Value v = Ort::Value::CreateTensor<float>(memInfo,
                                                      innerVector[i+10].data(), 
                                                      innerVector[i+10].size(), 
                                                      s.data(), 
                                                      s.size());
      cached_key_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], left_context_len_[i], 1,
                               attention_dims_[i] / 2};
      Ort::Value v = Ort::Value::CreateTensor<float>(memInfo,
                                                      innerVector[i+15].data(), 
                                                      innerVector[i+15].size(), 
                                                      s.data(), 
                                                      s.size());
      cached_val_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], left_context_len_[i], 1,
                               attention_dims_[i] / 2};
      Ort::Value v = Ort::Value::CreateTensor<float>(memInfo,
                                                      innerVector[i+20].data(), 
                                                      innerVector[i+20].size(), 
                                                      s.data(), 
                                                      s.size());
      cached_val2_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], 1, encoder_dims_[i],
                               cnn_module_kernels_[i] - 1};
      Ort::Value v = Ort::Value::CreateTensor<float>(memInfo,
                                                      innerVector[i+25].data(), 
                                                      innerVector[i+25].size(), 
                                                      s.data(), 
                                                      s.size());
      cached_conv1_vec.push_back(std::move(v));
    }

    {
      std::array<int64_t, 4> s{num_encoder_layers_[i], 1, encoder_dims_[i],
                               cnn_module_kernels_[i] - 1};
      Ort::Value v = Ort::Value::CreateTensor<float>(memInfo,
                                                      innerVector[i+30].data(), 
                                                      innerVector[i+30].size(), 
                                                      s.data(), 
                                                      s.size());
      cached_conv2_vec.push_back(std::move(v));
    }
  }

  std::vector<Ort::Value> ans;
  ans.reserve(n * 7);

  for (auto &v : cached_len_vec) ans.push_back(std::move(v));
  for (auto &v : cached_avg_vec) ans.push_back(std::move(v));
  for (auto &v : cached_key_vec) ans.push_back(std::move(v));
  for (auto &v : cached_val_vec) ans.push_back(std::move(v));
  for (auto &v : cached_val2_vec) ans.push_back(std::move(v));
  for (auto &v : cached_conv1_vec) ans.push_back(std::move(v));
  for (auto &v : cached_conv2_vec) ans.push_back(std::move(v));

  return ans;
}




std::pair<Ort::Value, std::vector<Ort::Value>> SNPEAsrModel::RunEncoder(
      Ort::Value features, std::vector<Ort::Value> states, Ort::Value processed_frames){
    //preprocessing for SNPE
    std::vector<float> encoder_input = ConvertOrtValueToVectorFloat(features);
    std::vector<std::vector<float>> encoder_input_states = FormatInputStates(states);
    // SHERPA_ONNX_LOGE("encoder_input_states 0 size: %zu", encoder_input_states[0].size());

    
    bool execStatus;
    zdl::DlSystem::UserBufferMap inputMap, outputMap;
    std::vector<float> outputVecEncoder;

    std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpeUserBackedInputBuffers, snpeUserBackedOutputBuffers;
    std::unordered_map <std::string, std::vector<float_t>> applicationOutputBuffers;

    // create UB_TF_N type buffer, if : useIntBuffer=True
    int bitWidth = 32;
    if(useIntBuffer)
        bitWidth = 8;  // set 16 for INT_16 activations

    // LOGI("Using UserBuffer with bit-width = %d", bitWidth);

    createOutputBufferMap(outputMap, applicationOutputBuffers, snpeUserBackedOutputBuffers, snpe_encoder, useIntBuffer, bitWidth);

    std::unordered_map <std::string, std::vector<float_t>> applicationInputBuffers;
    createInputBufferMap(inputMap, applicationInputBuffers, snpeUserBackedInputBuffers, snpe_encoder, useIntBuffer, bitWidth);
    if(!loadInputUserBuffer(applicationInputBuffers, snpe_encoder, encoder_input, encoder_input.size(), encoder_input_states, inputMap, bitWidth))
        LOGI("Failed to load Input UserBuffer");
//
//    // Execute the input buffer map on the model with SNPE
    execStatus = snpe_encoder->execute(inputMap, outputMap);
//    // Save the execution results only if successful
    if (execStatus == true) {
        // LOGI("SNPE Exec Success !!!");
        // save output tensor
        auto pair = saveOutput(outputMap, applicationOutputBuffers);
        outputVecEncoder = pair.first;
        encoder_input_states = pair.second;


    } else LOGI("\nSNPE Execute Failed\n");



    //post processing
    Ort::Value output = ConvertVectorToOrtValue(outputVecEncoder);
    std::vector<Ort::Value> new_states = FormatEncoderOutputStates(encoder_input_states);

    return {std::move(output), std::move(new_states)};
}

Ort::Value SNPEAsrModel::RunDecoder(
    Ort::Value decoder_input) {
      // LOGI("Running decoder...");
  auto decoder_out = decoder_sess_->Run(
      {}, decoder_input_names_ptr_.data(), &decoder_input, 1,
      decoder_output_names_ptr_.data(), decoder_output_names_ptr_.size());

      // LOGI("Successfully ran decoder.");
  return std::move(decoder_out[0]);
}

Ort::Value SNPEAsrModel::RunJoiner(Ort::Value encoder_out,
                                                      Ort::Value decoder_out) {

      // LOGI("Running joiner...");
  std::array<Ort::Value, 2> joiner_input = {std::move(encoder_out),
                                            std::move(decoder_out)};
  auto logit =
      joiner_sess_->Run({}, joiner_input_names_ptr_.data(), joiner_input.data(),
                        joiner_input.size(), joiner_output_names_ptr_.data(),
                        joiner_output_names_ptr_.size());

      // LOGI("Successfully ran joiner.");
  return std::move(logit[0]);
}


}  // namespace reverie
