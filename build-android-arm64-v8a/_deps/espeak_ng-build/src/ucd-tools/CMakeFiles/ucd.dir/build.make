# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/Bibekananda-PC/miniconda3/lib/python3.11/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/Bibekananda-PC/miniconda3/lib/python3.11/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a

# Include any dependencies generated for this target.
include _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/flags.make

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/case.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/flags.make
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/case.c.o: _deps/espeak_ng-src/src/ucd-tools/src/case.c
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/case.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/case.c.o"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/case.c.o -MF CMakeFiles/ucd.dir/src/case.c.o.d -o CMakeFiles/ucd.dir/src/case.c.o -c /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/case.c

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/case.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ucd.dir/src/case.c.i"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/case.c > CMakeFiles/ucd.dir/src/case.c.i

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/case.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ucd.dir/src/case.c.s"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/case.c -o CMakeFiles/ucd.dir/src/case.c.s

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/categories.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/flags.make
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/categories.c.o: _deps/espeak_ng-src/src/ucd-tools/src/categories.c
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/categories.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/categories.c.o"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/categories.c.o -MF CMakeFiles/ucd.dir/src/categories.c.o.d -o CMakeFiles/ucd.dir/src/categories.c.o -c /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/categories.c

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/categories.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ucd.dir/src/categories.c.i"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/categories.c > CMakeFiles/ucd.dir/src/categories.c.i

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/categories.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ucd.dir/src/categories.c.s"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/categories.c -o CMakeFiles/ucd.dir/src/categories.c.s

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/ctype.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/flags.make
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/ctype.c.o: _deps/espeak_ng-src/src/ucd-tools/src/ctype.c
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/ctype.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/ctype.c.o"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/ctype.c.o -MF CMakeFiles/ucd.dir/src/ctype.c.o.d -o CMakeFiles/ucd.dir/src/ctype.c.o -c /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/ctype.c

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/ctype.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ucd.dir/src/ctype.c.i"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/ctype.c > CMakeFiles/ucd.dir/src/ctype.c.i

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/ctype.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ucd.dir/src/ctype.c.s"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/ctype.c -o CMakeFiles/ucd.dir/src/ctype.c.s

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/proplist.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/flags.make
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/proplist.c.o: _deps/espeak_ng-src/src/ucd-tools/src/proplist.c
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/proplist.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/proplist.c.o"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/proplist.c.o -MF CMakeFiles/ucd.dir/src/proplist.c.o.d -o CMakeFiles/ucd.dir/src/proplist.c.o -c /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/proplist.c

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/proplist.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ucd.dir/src/proplist.c.i"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/proplist.c > CMakeFiles/ucd.dir/src/proplist.c.i

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/proplist.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ucd.dir/src/proplist.c.s"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/proplist.c -o CMakeFiles/ucd.dir/src/proplist.c.s

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/scripts.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/flags.make
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/scripts.c.o: _deps/espeak_ng-src/src/ucd-tools/src/scripts.c
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/scripts.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/scripts.c.o"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/scripts.c.o -MF CMakeFiles/ucd.dir/src/scripts.c.o.d -o CMakeFiles/ucd.dir/src/scripts.c.o -c /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/scripts.c

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/scripts.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ucd.dir/src/scripts.c.i"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/scripts.c > CMakeFiles/ucd.dir/src/scripts.c.i

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/scripts.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ucd.dir/src/scripts.c.s"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/scripts.c -o CMakeFiles/ucd.dir/src/scripts.c.s

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/tostring.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/flags.make
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/tostring.c.o: _deps/espeak_ng-src/src/ucd-tools/src/tostring.c
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/tostring.c.o: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/tostring.c.o"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/tostring.c.o -MF CMakeFiles/ucd.dir/src/tostring.c.o.d -o CMakeFiles/ucd.dir/src/tostring.c.o -c /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/tostring.c

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/tostring.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/ucd.dir/src/tostring.c.i"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/tostring.c > CMakeFiles/ucd.dir/src/tostring.c.i

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/tostring.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/ucd.dir/src/tostring.c.s"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android34 --sysroot=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sdk/ndk/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools/src/tostring.c -o CMakeFiles/ucd.dir/src/tostring.c.s

# Object files for target ucd
ucd_OBJECTS = \
"CMakeFiles/ucd.dir/src/case.c.o" \
"CMakeFiles/ucd.dir/src/categories.c.o" \
"CMakeFiles/ucd.dir/src/ctype.c.o" \
"CMakeFiles/ucd.dir/src/proplist.c.o" \
"CMakeFiles/ucd.dir/src/scripts.c.o" \
"CMakeFiles/ucd.dir/src/tostring.c.o"

# External object files for target ucd
ucd_EXTERNAL_OBJECTS =

lib/libucd.so: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/case.c.o
lib/libucd.so: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/categories.c.o
lib/libucd.so: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/ctype.c.o
lib/libucd.so: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/proplist.c.o
lib/libucd.so: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/scripts.c.o
lib/libucd.so: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/src/tostring.c.o
lib/libucd.so: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/build.make
lib/libucd.so: _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C shared library ../../../../lib/libucd.so"
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ucd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/build: lib/libucd.so
.PHONY : _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/build

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/clean:
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools && $(CMAKE_COMMAND) -P CMakeFiles/ucd.dir/cmake_clean.cmake
.PHONY : _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/clean

_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/depend:
	cd /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-src/src/ucd-tools /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools /media/Bibekananda-PC/c7c38480-8b5a-4d19-ad07-57124994f819/android/sherpa-onnx-qnn/build-android-arm64-v8a/_deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : _deps/espeak_ng-build/src/ucd-tools/CMakeFiles/ucd.dir/depend

