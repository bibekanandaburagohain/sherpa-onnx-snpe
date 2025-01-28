//
// Created by shubpate on 12/11/2021.
//

#ifndef NATIVEINFERENCE_INFERENCE_H
#define NATIVEINFERENCE_INFERENCE_H

std::string build_network(const uint8_t * dlc_buffer, const size_t dlc_size);
bool SetAdspLibraryPath(std::string nativeLibPath);
std::string execute_net(std::vector<float *> inputVec, int arrayLength,
                        std::vector<float *> & outputVec, std::string runtime);

#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/TensorShape.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/TensorMap.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/TensorShapeMap.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/IUserBufferFactory.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/IUserBuffer.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/UserBufferMap.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/IBufferAttributes.hpp"

#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/StringList.hpp"

#include "sherpa-onnx/csrc/snpe_inc/zdl/SNPE/SNPE.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/SNPE/SNPEFactory.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/DlVersion.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/DlEnums.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/String.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlContainer/IDlContainer.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/SNPE/SNPEBuilder.hpp"

#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/ITensor.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/ITensorFactory.hpp"

#include <unordered_map>
#include "android/log.h"

#define  LOG_TAG    "SNPE_INF"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

bool loadInputUserBuffer(std::unordered_map<std::string, std::vector<float_t>>& applicationBuffers,
                            std::unique_ptr<zdl::SNPE::SNPE>& snpe,
                            std::vector<float> inVector,
                            int arrayLength,
                            std::vector<std::vector<float>> model_cache_,
                            zdl::DlSystem::UserBufferMap& inputMap,
                            int bitWidth);

bool loadDecoderInputBuffer(std::unordered_map<std::string, std::vector<int64_t>>& applicationBuffers,
                         std::unique_ptr<zdl::SNPE::SNPE>& snpe,
                         std::vector<int64_t> inVector,
                         int arrayLength,
                         zdl::DlSystem::UserBufferMap& inputMap,
                         int bitWidth);
bool loadJoinerInputUserBuffer(std::unordered_map<std::string, std::vector<float>>& applicationBuffers,
                               std::unique_ptr<zdl::SNPE::SNPE>& snpe,
                               std::vector<float> inVector,
                               int arrayLength,
                               std::vector<float> inVectorDecoder,
                               int arrayLengthDecoder,
                               zdl::DlSystem::UserBufferMap& inputMap,
                               int bitWidth);

std::pair<std::vector<float>, std::vector<std::vector<float>>> saveOutput (zdl::DlSystem::UserBufferMap& outputMap,
                 std::unordered_map<std::string,std::vector<float_t>>& applicationOutputBuffers);

#endif //NATIVEINFERENCE_INFERENCE_H
