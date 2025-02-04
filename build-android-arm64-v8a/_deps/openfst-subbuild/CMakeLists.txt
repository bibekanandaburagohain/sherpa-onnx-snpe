# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.29.2)

# Reject any attempt to use a toolchain file. We must not use one because
# we could be downloading it here. If the CMAKE_TOOLCHAIN_FILE environment
# variable is set, the cache variable will have been initialized from it.
unset(CMAKE_TOOLCHAIN_FILE CACHE)
unset(ENV{CMAKE_TOOLCHAIN_FILE})

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(openfst-populate NONE)



include(ExternalProject)
ExternalProject_Add(openfst-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "FALSE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://github.com/kkm000/openfst/archive/refs/tags/win/1.6.5.1.tar.gz" "https://huggingface.co/csukuangfj/kaldi-hmm-gmm-cmake-deps/resolve/main/openfst-win-1.6.5.1.tar.gz" "URL_HASH" "SHA256=02c49b559c3976a536876063369efc0e41ab374be1035918036474343877046e" "PATCH_COMMAND" "sed" "-i.bak" "s/enable_testing()//g" "src/CMakeLists.txt" "&&" "sed" "-i.bak" "s/add_subdirectory(test)//g" "src/CMakeLists.txt" "&&" "sed" "-i.bak" "/message/d" "src/script/CMakeLists.txt"
                    SOURCE_DIR          "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/openfst-src"
                    BINARY_DIR          "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/openfst-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


