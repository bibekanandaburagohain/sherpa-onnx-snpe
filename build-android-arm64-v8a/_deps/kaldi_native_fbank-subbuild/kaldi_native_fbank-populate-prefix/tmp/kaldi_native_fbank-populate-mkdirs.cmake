# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldi_native_fbank-src"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldi_native_fbank-build"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/tmp"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src/kaldi_native_fbank-populate-stamp"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src"
  "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src/kaldi_native_fbank-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src/kaldi_native_fbank-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src/kaldi_native_fbank-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
