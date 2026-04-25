@echo off
setlocal

echo [1/6] Cleaning old smoke-test folders...
if exist build\smoke-producer-ninja rmdir /s /q build\smoke-producer-ninja
if exist build\smoke-consumer-ninja rmdir /s /q build\smoke-consumer-ninja

echo [2/6] Configuring producer...
cmake -S examples\11-install-export-package -B build\smoke-producer-ninja -G Ninja
if errorlevel 1 exit /b 1

echo [3/6] Building producer...
cmake --build build\smoke-producer-ninja
if errorlevel 1 exit /b 1

echo [4/6] Installing producer...
cmake --install build\smoke-producer-ninja --prefix build\smoke-producer-ninja\install
if errorlevel 1 exit /b 1

echo [5/6] Configuring consumer...
cmake -S examples\11-install-export-package\consumer ^
      -B build\smoke-consumer-ninja ^
      -G Ninja ^
      -DTUTORIAL_MATH_CONFIG_DIR="build/smoke-producer-ninja/install/lib/cmake/TutorialMath"
if errorlevel 1 exit /b 1

echo [6/6] Building and running consumer...
cmake --build build\smoke-consumer-ninja
if errorlevel 1 exit /b 1

build\smoke-consumer-ninja\tutorial_math_consumer.exe
if errorlevel 1 exit /b 1

echo Smoke test passed.
endlocal