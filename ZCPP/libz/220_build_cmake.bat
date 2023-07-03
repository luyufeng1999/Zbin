@echo off
setlocal enabledelayedexpansion
set ABIS=arm64-v8a
set ANDROID_NDK=D:\Android\sdk\ndk\23.1.7779620
set CMAKE=D:\Android\sdk\cmake\3.22.1\bin\cmake.exe
cd %~dp0

for %%i in (%ABIS%) do (
  set ABI=%%i
  set BUILD_DIR=_build_%%i
  if not exist !BUILD_DIR! mkdir !BUILD_DIR!
  cd !BUILD_DIR!
  %CMAKE% -G "Ninja" ^
    -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\build\cmake\android.toolchain.cmake ^
    -DANDROID_ABI=%%i ^
    -DANDROID_PLATFORM=24 ^
    -DANDROID_NATIVE_API_LEVEL=19 ^
    -DCMAKE_MAKE_PROGRAM=D:\Android\sdk\cmake\3.18.1\bin\ninja.exe ^
    -DANDROID_STL=c++_static ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DCMAKE_C_FLAGS_RELEASE="-O3" ^
    -DCMAKE_CXX_FLAGS_RELEASE="-O3" ^
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=%~dp0\built\%%i ^
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=%~dp0\built\%%i ^
    ..
  %CMAKE% --build .
  cd ..
)