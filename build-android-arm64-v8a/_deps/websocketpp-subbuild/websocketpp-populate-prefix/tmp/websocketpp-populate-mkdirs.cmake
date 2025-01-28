# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/websocketpp-src"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/websocketpp-build"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/websocketpp-subbuild/websocketpp-populate-prefix"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/websocketpp-subbuild/websocketpp-populate-prefix/tmp"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/websocketpp-subbuild/websocketpp-populate-prefix/src/websocketpp-populate-stamp"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/websocketpp-subbuild/websocketpp-populate-prefix/src"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/websocketpp-subbuild/websocketpp-populate-prefix/src/websocketpp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/websocketpp-subbuild/websocketpp-populate-prefix/src/websocketpp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/websocketpp-subbuild/websocketpp-populate-prefix/src/websocketpp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
