# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldifst-src"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldifst-build"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldifst-subbuild/kaldifst-populate-prefix"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldifst-subbuild/kaldifst-populate-prefix/tmp"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldifst-subbuild/kaldifst-populate-prefix/src/kaldifst-populate-stamp"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldifst-subbuild/kaldifst-populate-prefix/src"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldifst-subbuild/kaldifst-populate-prefix/src/kaldifst-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldifst-subbuild/kaldifst-populate-prefix/src/kaldifst-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldifst-subbuild/kaldifst-populate-prefix/src/kaldifst-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
