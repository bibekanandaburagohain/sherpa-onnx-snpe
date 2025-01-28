// Copyright (c) 2020 Mobvoi Inc (Binbin Zhang, Di Wu)
//               2022 ZeXuan Li (lizexuan@huya.com)
//                    Xingchen Song(sxc19@mails.tsinghua.edu.cn)
//                    hamddct@gmail.com (Mddct)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CSRC_SNPE_ASR_MODEL_H_
#define CSRC_SNPE_ASR_MODEL_H_

#include <memory>
#include <string>
#include <vector>
#include "log.h"
#include "utils.h"

#if __ANDROID_API__ >= 9
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#endif
#include "sherpa-onnx/csrc/snpe_inc/hpp/CheckRuntime.hpp"
#include "sherpa-onnx/csrc/snpe_inc/hpp/LoadContainer.hpp"
#include "sherpa-onnx/csrc/snpe_inc/hpp/SetBuilderOptions.hpp"
#include "sherpa-onnx/csrc/snpe_inc/hpp/LoadInputTensor.hpp"
#include "sherpa-onnx/csrc/snpe_inc/hpp/CreateUserBuffer.hpp"
#include "sherpa-onnx/csrc/snpe_inc/hpp/Util.hpp"
#include "onnxruntime_cxx_api.h"  // NOLINT
#include "sherpa-onnx/csrc/online-model-config.h"
#include "sherpa-onnx/csrc/online-transducer-model.h"

namespace sherpa_onnx {

class SNPEAsrModel : public OnlineTransducerModel {
 public:
  explicit SNPEAsrModel(const OnlineModelConfig &config);

#if __ANDROID_API__ >= 9
  SNPEAsrModel(AAssetManager *mgr,
                                  const OnlineModelConfig &config);
#endif

  std::vector<Ort::Value> StackStates(
      const std::vector<std::vector<Ort::Value>> &states) const override;

  std::vector<std::vector<Ort::Value>> UnStackStates(
      const std::vector<Ort::Value> &states) const override;

  std::vector<Ort::Value> GetEncoderInitStates() override;
  std::vector<Ort::Value> FormatEncoderOutputStates(std::vector<std::vector<float>> inputVector);
  std::vector<std::vector<float>> FormatInputStates(const std::vector<Ort::Value> &states);

  void SetFeatureDim(int32_t feature_dim) override {
    feature_dim_ = feature_dim;
  }

  std::pair<Ort::Value, std::vector<Ort::Value>> RunEncoder(
      Ort::Value features, std::vector<Ort::Value> states,
      Ort::Value processed_frames) override;
    
  Ort::Value RunDecoder(Ort::Value decoder_input) override;

  Ort::Value RunJoiner(Ort::Value encoder_out, Ort::Value decoder_out) override;

  int32_t ContextSize() const override { return context_size_; }

  int32_t ChunkSize() const override { return T_; }

  int32_t ChunkShift() const override { return decode_chunk_len_; }

  int32_t VocabSize() const override { return vocab_size_; }
  OrtAllocator *Allocator() override { return allocator_; }

 private:
  void InitEncoder(AAssetManager *mgr, std::string encoder_name);
  void InitDecoder(void *model_data, size_t model_data_length);
  void InitJoiner(void *model_data, size_t model_data_length);
  Ort::Value ConvertVectorToOrtValue(const std::vector<float>& inputVector);

 private:
  Ort::Env env_;
  Ort::SessionOptions sess_opts_;
  Ort::AllocatorWithDefaultOptions allocator_;

  std::unique_ptr<Ort::Session> encoder_sess_;
  std::unique_ptr<Ort::Session> decoder_sess_;
  std::unique_ptr<Ort::Session> joiner_sess_;

  std::vector<std::string> encoder_input_names_;
  std::vector<const char *> encoder_input_names_ptr_;

  std::vector<std::string> encoder_output_names_;
  std::vector<const char *> encoder_output_names_ptr_;

  std::vector<std::string> decoder_input_names_;
  std::vector<const char *> decoder_input_names_ptr_;

  std::vector<std::string> decoder_output_names_;
  std::vector<const char *> decoder_output_names_ptr_;

  std::vector<std::string> joiner_input_names_;
  std::vector<const char *> joiner_input_names_ptr_;

  std::vector<std::string> joiner_output_names_;
  std::vector<const char *> joiner_output_names_ptr_;

  OnlineModelConfig config_;

  std::vector<int32_t> encoder_dims_ = {128,128,128,128,128};
  std::vector<int32_t> attention_dims_ = {96,96,96,96,96};
  std::vector<int32_t> num_encoder_layers_ = {2,2,2,2,2};
  std::vector<int32_t> cnn_module_kernels_ = {31,31,31,31,31};
  std::vector<int32_t> left_context_len_ = {64,32,16,8,32};


  int32_t T_ = 39;
  int32_t decode_chunk_len_ = 32;

  int32_t context_size_ = 2;
  int32_t vocab_size_ = 512;
  int32_t feature_dim_ = 80;

};

}  // namespace sherpa_onnx

#endif  // SHERPA_ONNX_CSRC_ONLINE_ZIPFORMER2_TRANSDUCER_MODEL_H_
