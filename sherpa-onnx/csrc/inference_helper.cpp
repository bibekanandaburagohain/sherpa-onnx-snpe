#include <jni.h>
#include <string>
#include <iostream>
#include "android/log.h"

#include "sherpa-onnx/csrc/snpe_inc/zdl/SNPE/SNPE.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/SNPE/SNPEFactory.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/DlVersion.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/DlEnums.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/String.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlContainer/IDlContainer.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/SNPE/SNPEBuilder.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/ITensor.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/StringList.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/TensorMap.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/TensorShape.hpp"
#include "sherpa-onnx/csrc/snpe_inc/zdl/DlSystem/ITensorFactory.hpp"

#include "sherpa-onnx/csrc/snpe_inc/hpp/LoadInputTensor.hpp"
#include "sherpa-onnx/csrc/snpe_inc/hpp/Util.hpp"
#include "sherpa-onnx/csrc/snpe_inc/hpp/inference.h"


bool SetAdspLibraryPath(std::string nativeLibPath) {
    nativeLibPath += ";/data/local/tmp/mv_dlc;/vendor/lib/rfsa/adsp;/vendor/dsp/cdsp;/system/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp";

    __android_log_print(ANDROID_LOG_INFO, "SNPE ", "ADSP Lib Path = %s \n", nativeLibPath.c_str());
    std::cout << "ADSP Lib Path = " << nativeLibPath << std::endl;

    return setenv("ADSP_LIBRARY_PATH", nativeLibPath.c_str(), 1 /*override*/) == 0;
}




std::unique_ptr<zdl::DlContainer::IDlContainer> loadContainerFromBuffer(const uint8_t * buffer, const size_t size)
{
    std::unique_ptr<zdl::DlContainer::IDlContainer> container;
    container = zdl::DlContainer::IDlContainer::open(buffer, size);
    return container;
}

zdl::DlSystem::Runtime_t checkRuntime(zdl::DlSystem::Runtime_t runtime)
{
    static zdl::DlSystem::Version_t Version = zdl::SNPE::SNPEFactory::getLibraryVersion();

    LOGI("SNPE Version = %s", Version.asString().c_str()); //Print Version number

    if (!zdl::SNPE::SNPEFactory::isRuntimeAvailable(runtime)) {
        LOGE("Selected runtime not present. Falling back to GPU.");
        runtime = zdl::DlSystem::Runtime_t::GPU;
        if (!zdl::SNPE::SNPEFactory::isRuntimeAvailable(runtime)){
            LOGE("Selected runtime not present. Falling back to CPU.");
            runtime = zdl::DlSystem::Runtime_t::CPU;
        }
    }

    return runtime;
}

std::unique_ptr<zdl::SNPE::SNPE> setBuilderOptions(std::unique_ptr<zdl::DlContainer::IDlContainer> & container,
                                                   zdl::DlSystem::Runtime_t runtime,
                                                   zdl::DlSystem::RuntimeList runtimeList,
                                                   bool useUserSuppliedBuffers,
                                                   bool useCaching)
{
    std::unique_ptr<zdl::SNPE::SNPE> snpe;
    zdl::SNPE::SNPEBuilder snpeBuilder(container.get());

    //runtimeList.add(runtime);


    std::string platformOptionStr = "useAdaptivePD:ON";

    zdl::DlSystem::PlatformConfig platformConfig;
    bool setSuccess = platformConfig.setPlatformOptions(platformOptionStr);
    if (!setSuccess)
        LOGE("=========> failed to set platformconfig: %s", platformOptionStr.c_str());
    else
        LOGI("=========> platformconfig set: %s", platformOptionStr.c_str());

    bool isValid = platformConfig.isOptionsValid();
    if (!isValid)
        LOGE("=========> platformconfig option is invalid");
    else
        LOGE("=========> platformconfig option: valid");

    zdl::DlSystem::StringList stringruntime = runtimeList.getRuntimeListNames();
    for (const char *name : stringruntime)
        LOGI("runtime sh %s", name);

    snpe = snpeBuilder.setOutputLayers({})
            .setPerformanceProfile(zdl::DlSystem::PerformanceProfile_t::BURST)
            .setExecutionPriorityHint(
                    zdl::DlSystem::ExecutionPriorityHint_t::HIGH)
            .setRuntimeProcessorOrder(runtimeList)
            .setUseUserSuppliedBuffers(useUserSuppliedBuffers)
            .setPlatformConfig(platformConfig)
            .setInitCacheMode(useCaching)
            .setUnconsumedTensorsAsOutputs(true)
            .build();

    return snpe;
}

std::unique_ptr<zdl::DlSystem::ITensor> loadInputTensor (std::unique_ptr<zdl::SNPE::SNPE>& snpe , std::vector<float>& inp_raw) {
    std::unique_ptr<zdl::DlSystem::ITensor> input;
    const auto &strList_opt = snpe->getInputTensorNames();
    if (!strList_opt) throw std::runtime_error("Error obtaining Input tensor names");
    const auto &strList = *strList_opt;
    // Make sure the network requires only a single input
    //assert (strList.size() == 1);

    const auto &inputDims_opt = snpe->getInputDimensions(strList.at(0));
    const auto &inputShape = *inputDims_opt;

    input = zdl::SNPE::SNPEFactory::getTensorFactory().createTensor(inputShape);

    if (input->getSize() != inp_raw.size()) {
        std::string errStr = "Size of input does not match network.\n Expecting: " + std::to_string(input->getSize());
        errStr +=  "; Got: " + std::to_string(inp_raw.size()) + "\n";
        LOGE("%s",errStr.c_str());
        return nullptr;
    }

    /* Copy the loaded input file contents into the networks input tensor.
    SNPE's ITensor supports C++ STL functions like std::copy() */
    std::copy(inp_raw.begin(), inp_raw.end(), input->begin());
    return input;
}

// ==============================User Buffer func=================================== //
// ================================================================================= //
size_t resizable_dim;

size_t calcSizeFromDims(const zdl::DlSystem::Dimension *dims, size_t rank, size_t elementSize ){
    if (rank == 0) return 0;
    size_t size = elementSize;
    while (rank--) {
        (*dims == 0) ? size *= resizable_dim : size *= *dims;
        dims++;
    }
    return size;
}

//CreateUserbuffer INPUT/OUTPUT

//Changed the float_32 class to float_t
void createUserBuffer(zdl::DlSystem::UserBufferMap& userBufferMap,
                      std::unordered_map<std::string, std::vector<float_t>>& applicationBuffers,
                      std::vector<std::unique_ptr<zdl::DlSystem::IUserBuffer>>& snpeUserBackedBuffers,
                      std::unique_ptr<zdl::SNPE::SNPE>& snpe,
                      const char * name,
                      const bool isTfNBuffer,
                      int bitWidth)
{

    auto bufferAttributesOpt = snpe->getInputOutputBufferAttributes(name);
    if (!bufferAttributesOpt) throw std::runtime_error(std::string("Error obtaining attributes for input tensor ") + name);

    // calculate the size of buffer required by the input tensor
    const zdl::DlSystem::TensorShape& bufferShape = (*bufferAttributesOpt)->getDims();

    size_t bufferElementSize = 0;
    if (isTfNBuffer) {
        bufferElementSize = bitWidth / 8;
    }
    else {
        bufferElementSize = sizeof(float);
    }

    // LOGI("ASRHelper: bufferElementSize %d",bufferElementSize);
    // Calculate the stride based on buffer strides.
    // Note: Strides = Number of bytes to advance to the next element in each dimension.
    // For example, if a float tensor of dimension 2x4x3 is tightly packed in a buffer of 96 bytes, then the strides would be (48,12,4)
    // Note: Buffer stride is usually known and does not need to be calculated.

//    1x128x128x3
//    [196608,1536,12,4]
    int num_dims = bufferShape.rank(); //bufferShape rank is generally 1 more than expected, as it add 1 for batchSize, so 320x320x3 will look like 1x320x320x3
    // LOGI("ASRHelper: num_dims %d",num_dims);
    std::vector<size_t> strides(num_dims);

    //stride [196608 1536 12 4]
    //buffershape [ 1 128 128 3]
    //stride 4*3*128
    strides[strides.size() - 1] = bufferElementSize;
    size_t stride = strides[strides.size() - 1];
    for (size_t i = num_dims - 1; i > 0; i--) {
        stride *= bufferShape[i];
        strides[i - 1] = stride;
        // LOGI("\n ASRHelper:inference_helper(for loop) strides[%d]: %d",i-1,stride);
        // LOGI("\nASRHelper:inference_helper(for loop) buffershape[%d]: %d",i,bufferShape[i]);
    }

    size_t bufSize=bufferElementSize;
    for(int i=0;i<num_dims;i++)
        bufSize*=bufferShape[i];

//    LOGI("\nASRHelper:BufferName: %s Bufsize: %d", name, bufSize);

    // set the buffer encoding type
    std::unique_ptr<zdl::DlSystem::UserBufferEncoding> userBufferEncoding;
    if (isTfNBuffer)
        userBufferEncoding = std::unique_ptr<zdl::DlSystem::UserBufferEncodingTfN>(
                new zdl::DlSystem::UserBufferEncodingTfN(0, 1.0, bitWidth));
    else
        userBufferEncoding = std::unique_ptr<zdl::DlSystem::UserBufferEncodingFloat>(
                new zdl::DlSystem::UserBufferEncodingFloat());

    // create user-backed storage to load input data onto it
    applicationBuffers.emplace(name, std::vector<float_t>(bufSize));

    // create SNPE user buffer from the user-backed buffer
    zdl::DlSystem::IUserBufferFactory &ubFactory = zdl::SNPE::SNPEFactory::getUserBufferFactory();
    snpeUserBackedBuffers.push_back(
            ubFactory.createUserBuffer(applicationBuffers.at(name).data(),
                                       bufSize,
                                       strides,
                                       userBufferEncoding.get()));
    if (snpeUserBackedBuffers.back() == nullptr)
        throw std::runtime_error(std::string("Error while creating user buffer."));

    // add the user-backed buffer to the inputMap, which is later on fed to the network for execution
    userBufferMap.add(name, snpeUserBackedBuffers.back().get());

}

void createUserBuffer(zdl::DlSystem::UserBufferMap& userBufferMap,
                      std::unordered_map<std::string, std::vector<int64_t>>& applicationBuffers,
                      std::vector<std::unique_ptr<zdl::DlSystem::IUserBuffer>>& snpeUserBackedBuffers,
                      std::unique_ptr<zdl::SNPE::SNPE>& snpe,
                      const char * name,
                      const bool isTfNBuffer,
                      int bitWidth)
{

    auto bufferAttributesOpt = snpe->getInputOutputBufferAttributes(name);
    if (!bufferAttributesOpt) throw std::runtime_error(std::string("Error obtaining attributes for input tensor ") + name);

    // calculate the size of buffer required by the input tensor
    const zdl::DlSystem::TensorShape& bufferShape = (*bufferAttributesOpt)->getDims();

    size_t bufferElementSize = 0;
    if (isTfNBuffer) {
        bufferElementSize = bitWidth / 8;
    }
    else {
        bufferElementSize = sizeof(float);
    }

    LOGI("ASRHelper: bufferElementSize %d",bufferElementSize);
    // Calculate the stride based on buffer strides.
    // Note: Strides = Number of bytes to advance to the next element in each dimension.
    // For example, if a float tensor of dimension 2x4x3 is tightly packed in a buffer of 96 bytes, then the strides would be (48,12,4)
    // Note: Buffer stride is usually known and does not need to be calculated.

//    1x128x128x3
//    [196608,1536,12,4]
    int num_dims = bufferShape.rank(); //bufferShape rank is generally 1 more than expected, as it add 1 for batchSize, so 320x320x3 will look like 1x320x320x3
    LOGI("ASRHelper: num_dims %d",num_dims);
    std::vector<size_t> strides(num_dims);

    //stride [196608 1536 12 4]
    //buffershape [ 1 128 128 3]
    //stride 4*3*128
    strides[strides.size() - 1] = bufferElementSize;
    size_t stride = strides[strides.size() - 1];
    for (size_t i = num_dims - 1; i > 0; i--) {
        stride *= bufferShape[i];
        strides[i - 1] = stride;
        LOGI("\n ASRHelper:inference_helper(for loop) strides[%d]: %d",i-1,stride);
        LOGI("\nASRHelper:inference_helper(for loop) buffershape[%d]: %d",i,bufferShape[i]);
    }

    size_t bufSize=bufferElementSize;
    for(int i=0;i<num_dims;i++)
        bufSize*=bufferShape[i];

    LOGI("\nASRHelper:BufferName: %s Bufsize: %d", name, bufSize);

    // set the buffer encoding type
    std::unique_ptr<zdl::DlSystem::UserBufferEncoding> userBufferEncoding;
    if (isTfNBuffer)
        userBufferEncoding = std::unique_ptr<zdl::DlSystem::UserBufferEncodingTfN>(
                new zdl::DlSystem::UserBufferEncodingTfN(0, 1.0, bitWidth));
    else
        userBufferEncoding = std::unique_ptr<zdl::DlSystem::UserBufferEncodingFloat>(
                new zdl::DlSystem::UserBufferEncodingFloat());

    // create user-backed storage to load input data onto it
    applicationBuffers.emplace(name, std::vector<int64_t>(bufSize));

    // create SNPE user buffer from the user-backed buffer
    zdl::DlSystem::IUserBufferFactory &ubFactory = zdl::SNPE::SNPEFactory::getUserBufferFactory();
    snpeUserBackedBuffers.push_back(
            ubFactory.createUserBuffer(applicationBuffers.at(name).data(),
                                       bufSize,
                                       strides,
                                       userBufferEncoding.get()));
    if (snpeUserBackedBuffers.back() == nullptr)
        throw std::runtime_error(std::string("Error while creating user buffer."));

    // add the user-backed buffer to the inputMap, which is later on fed to the network for execution
    userBufferMap.add(name, snpeUserBackedBuffers.back().get());

}


void createOutputBufferMap(zdl::DlSystem::UserBufferMap &outputMap,
                           std::unordered_map<std::string, std::vector<float_t>> &applicationBuffers,
                           std::vector<std::unique_ptr<zdl::DlSystem::IUserBuffer>> &snpeUserBackedBuffers,
                           std::unique_ptr<zdl::SNPE::SNPE> &snpe,
                           bool isTfNBuffer,
                           int bitWidth)
{
    //LOGI("Creating Output Buffer");
    const auto& outputNamesOpt = snpe->getOutputTensorNames();
    if (!outputNamesOpt) throw std::runtime_error("Error obtaining output tensor names");
    const zdl::DlSystem::StringList& outputNames = *outputNamesOpt;

    // create SNPE user buffers for each application storage buffer
    for (const char *name : outputNames) {
        // LOGI("Creating output buffer %s", name);
        createUserBuffer(outputMap, applicationBuffers, snpeUserBackedBuffers, snpe, name, isTfNBuffer, bitWidth);
    }
}

void createInputBufferMap(zdl::DlSystem::UserBufferMap& inputMap,
                          std::unordered_map<std::string, std::vector<float_t>>& applicationBuffers,
                          std::vector<std::unique_ptr<zdl::DlSystem::IUserBuffer>>& snpeUserBackedBuffers,
                          std::unique_ptr<zdl::SNPE::SNPE>& snpe,
                          bool isTfNBuffer,
                          int bitWidth) {
    //LOGI("Creating Input Buffer");
    const auto &inputNamesOpt = snpe->getInputTensorNames();
    if (!inputNamesOpt) throw std::runtime_error("Error obtaining input tensor names");
    const zdl::DlSystem::StringList &inputNames = *inputNamesOpt;


    // create SNPE user buffers for each application storage buffer
    for (const char *name: inputNames) {
        // LOGI("Creating Input Buffer = %s", name);
        createUserBuffer(inputMap, applicationBuffers, snpeUserBackedBuffers, snpe, name,
                         isTfNBuffer, bitWidth);
    }
}

void createInputBufferMap(zdl::DlSystem::UserBufferMap& inputMap,
                          std::unordered_map<std::string, std::vector<int64_t>>& applicationBuffers,
                          std::vector<std::unique_ptr<zdl::DlSystem::IUserBuffer>>& snpeUserBackedBuffers,
                          std::unique_ptr<zdl::SNPE::SNPE>& snpe,
                          bool isTfNBuffer,
                          int bitWidth) {
    //LOGI("Creating Input Buffer");
    const auto &inputNamesOpt = snpe->getInputTensorNames();
    if (!inputNamesOpt) throw std::runtime_error("Error obtaining input tensor names");
    const zdl::DlSystem::StringList &inputNames = *inputNamesOpt;


    // create SNPE user buffers for each application storage buffer
    for (const char *name: inputNames) {
        // LOGI("Creating Input Buffer = %s", name);
        createUserBuffer(inputMap, applicationBuffers, snpeUserBackedBuffers, snpe, name,
                         isTfNBuffer, bitWidth);
    }
}

bool FloatToTfN(float_t* out,
                unsigned char& stepEquivalentTo0,
                float& quantizedStepSize,
                float* in,
                size_t numElement,
                int bitWidth)
{
    float trueMin = std::numeric_limits <float>::max();
    float trueMax = std::numeric_limits <float>::min();

    for (size_t i = 0; i < numElement; ++i) {
        trueMin = fmin(trueMin, in[i]);
        trueMax = fmax(trueMax, in[i]);
    }

    double encodingMin;
    double encodingMax;
    double stepCloseTo0;
    double trueBitWidthMax = pow(2, bitWidth) -1;

    if (trueMin > 0.0f) {
        stepCloseTo0 = 0.0;
        encodingMin = 0.0;
        encodingMax = trueMax;
    } else if (trueMax < 0.0f) {
        stepCloseTo0 = trueBitWidthMax;
        encodingMin = trueMin;
        encodingMax = 0.0;
    } else {
        double trueStepSize = static_cast <double>(trueMax - trueMin) / trueBitWidthMax;
        stepCloseTo0 = -trueMin / trueStepSize;
        if (stepCloseTo0==round(stepCloseTo0)) {
            // 0.0 is exactly representable
            encodingMin = trueMin;
            encodingMax = trueMax;
        } else {
            stepCloseTo0 = round(stepCloseTo0);
            encodingMin = (0.0 - stepCloseTo0) * trueStepSize;
            encodingMax = (trueBitWidthMax - stepCloseTo0) * trueStepSize;
        }
    }

    const double minEncodingRange = 0.01;
    double encodingRange = encodingMax - encodingMin;
    quantizedStepSize = encodingRange / trueBitWidthMax;
    stepEquivalentTo0 = static_cast <unsigned char> (round(stepCloseTo0));

    if (encodingRange < minEncodingRange) {
        LOGE("Expect the encoding range to be larger than %f", minEncodingRange);
        LOGE("Got: %f", encodingRange);
        return false;
    } else {
        for (size_t i = 0; i < numElement; ++i) {
            int quantizedValue = round(trueBitWidthMax * (in[i] - encodingMin) / encodingRange);

            if (quantizedValue < 0)
                quantizedValue = 0;
            else if (quantizedValue > (int)trueBitWidthMax)
                quantizedValue = (int)trueBitWidthMax;

            if(bitWidth == 8){
                out[i] = static_cast <uint8_t> (quantizedValue);
            }
            else if(bitWidth == 16){
                uint16_t *temp = (uint16_t *)out;
                temp[i] = static_cast <uint16_t> (quantizedValue);
            }
        }
    }
    return true;
}

bool loadByteDataFileBatchedTfN(float *inVector, int arrayLength, std::vector<float_t>& loadVector, size_t offset,
                                unsigned char& stepEquivalentTo0, float& quantizedStepSize, int bitWidth)
{
    // loadVector = applicationBuffer
    size_t dataStartPos = 0;
    if(!FloatToTfN(&loadVector[dataStartPos], stepEquivalentTo0, quantizedStepSize, inVector, arrayLength, bitWidth))
        return false;

    return true;
}

bool loadInputUserBuffer(std::unordered_map<std::string, std::vector<float_t>>& applicationBuffers,
                         std::unique_ptr<zdl::SNPE::SNPE>& snpe,
                         std::vector<float> inVector,
                         int arrayLength,
                         std::vector<std::vector<float>> model_cache_,
                         zdl::DlSystem::UserBufferMap& inputMap,
                         int bitWidth) {
    // get input tensor names of the network that need to be populated
    const auto &inputNamesOpt = snpe->getInputTensorNames();
    if (!inputNamesOpt) throw std::runtime_error("Error obtaining input tensor names");
    const zdl::DlSystem::StringList &inputNames = *inputNamesOpt;


    // if (inputNames.size()) LOGI("ASRHelper: Processing DNN Input: ");

    for (size_t j = 0; j < inputNames.size(); j++) {
        const char *name = inputNames.at(j);
        // LOGI("ASRHelper: Filling %s buffer ", name);

        if(bitWidth == 8 || bitWidth == 16) {
            // load user-buffer tf-N
            LOGE("bit-width 8 and 16 are NOT DEFINED");
            return false;
        } else {
            // load user-buffer float
            std::vector<float_t> &loadVector = applicationBuffers.at(name);
            // std::string encoder_inp = "x";
            if(name == std::string("x")) {
            //    LOGE("ASRHelper:arrayLength size: %d", arrayLength);
                loadVector.resize(arrayLength * sizeof(float_t));

                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < arrayLength; ++idx) {
                    accumulator[idx] = inVector[idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_len_0")){
                int cache_idx = 0;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], model_cache_[cache_idx][idx]);
                    // }

                }
            }
            else if(name == std::string("cached_len_1")){
                int cache_idx = 1;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], model_cache_[cache_idx][idx]);
                    // }

                }
            }
            else if(name == std::string("cached_len_2")){
                int cache_idx = 2;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_len_3")){
                int cache_idx = 3;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_len_4")){
                int cache_idx = 4;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            
            else if(name == std::string("cached_avg_0")){
                int cache_idx = 5;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_avg_1")){
                int cache_idx = 6;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_avg_2")){
                int cache_idx = 7;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_avg_3")){
                int cache_idx = 8;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_avg_4")){
                int cache_idx = 9;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            
            else if(name == std::string("cached_key_0")){
                int cache_idx = 10;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_key_1")){
                int cache_idx = 11;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_key_2")){
                int cache_idx = 12;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_key_3")){
                int cache_idx = 13;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_key_4")){
                int cache_idx = 14;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }

            else if(name == std::string("cached_val_0")){
                int cache_idx = 15;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_val_1")){
                int cache_idx = 16;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_val_2")){
                int cache_idx = 17;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_val_3")){
                int cache_idx = 18;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_val_4")){
                int cache_idx = 19;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }

            else if(name == std::string("cached_val2_0")){
                int cache_idx = 20;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_val2_1")){
                int cache_idx = 21;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_val2_2")){
                int cache_idx = 22;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_val2_3")){
                int cache_idx = 23;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_val2_4")){
                int cache_idx = 24;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }

            else if(name == std::string("cached_conv1_0")){
                int cache_idx = 25;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_conv1_1")){
                int cache_idx = 26;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_conv1_2")){
                int cache_idx = 27;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_conv1_3")){
                int cache_idx = 28;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_conv1_4")){
                int cache_idx = 29;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }

            else if(name == std::string("cached_conv2_0")){
                int cache_idx = 30;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_conv2_1")){
                int cache_idx = 31;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_conv2_2")){
                int cache_idx = 32;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_conv2_3")){
                int cache_idx = 33;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
            else if(name == std::string("cached_conv2_4")){
                int cache_idx = 34;
                loadVector.resize(model_cache_[cache_idx].size() * sizeof(float_t));
                float *accumulator = reinterpret_cast<float *> (&loadVector[0]);
                for (int idx = 0; idx < model_cache_[cache_idx].size(); ++idx) {
                    accumulator[idx] = model_cache_[cache_idx][idx];
                    // if (idx < 4) {
                    //     LOGI("ASRHelper: %d -> %f,%f", idx, accumulator[idx], inVector[idx]);
                    // }

                }
            }
        }
    }
    return true;
}

// ======================================SAve UB Buffers==================== //
// ========================================================================= //
void TfNToFloat(float *out,
                uint8_t *in,
                const unsigned char stepEquivalentTo0,
                const float quantizedStepSize,
                size_t numElement,
                int bitWidth)
{
    for (size_t i = 0; i < numElement; ++i) {
        if (8 == bitWidth) {
            double quantizedValue = static_cast <double> (in[i]);
            double stepEqTo0 = static_cast <double> (stepEquivalentTo0);
            out[i] = static_cast <double> ((quantizedValue - stepEqTo0) * quantizedStepSize);
        }
        else if (16 == bitWidth) {
            uint16_t *temp = (uint16_t *)in;
            double quantizedValue = static_cast <double> (temp[i]);
            double stepEqTo0 = static_cast <double> (stepEquivalentTo0);
            out[i] = static_cast <double> ((quantizedValue - stepEqTo0) * quantizedStepSize);
        }
    }
}

std::pair<std::vector<float>, std::vector<std::vector<float>>> saveOutput (zdl::DlSystem::UserBufferMap& outputMap,
                 std::unordered_map<std::string,std::vector<float_t>>& applicationOutputBuffers) {
    // Get all output buffer names from the network
    const zdl::DlSystem::StringList &outputBufferNames = outputMap.getUserBufferNames();

    std::vector<std::vector<float>> model_cache_;
    model_cache_.clear();

    std::vector<float> outputVec = applicationOutputBuffers.at("encoder_out");
    
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_len_0"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_len_1"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_len_2"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_len_3"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_len_4"));

    model_cache_.push_back(applicationOutputBuffers.at("new_cached_avg_0"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_avg_1"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_avg_2"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_avg_3"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_avg_4"));

    model_cache_.push_back(applicationOutputBuffers.at("new_cached_key_0"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_key_1"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_key_2"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_key_3"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_key_4"));

    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val_0"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val_1"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val_2"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val_3"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val_4"));

    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val2_0"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val2_1"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val2_2"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val2_3"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_val2_4"));

    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv1_0"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv1_1"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv1_2"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv1_3"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv1_4"));

    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv2_0"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv2_1"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv2_2"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv2_3"));
    model_cache_.push_back(applicationOutputBuffers.at("new_cached_conv2_4"));



    return {std::move(outputVec), std::move(model_cache_)};

}
