I used your current `ExamplesHowTo.md` as the base and completed it from Chapter 08 onward, while keeping the same style and structure. 
# Examples - Build, Run, and Test Guide
This document collects the configure, build, run, and test commands for all examples created so far.
Covered examples:
- `01-first-project`
- `02-build-systems-and-generators`
- `03-libraries`
- `04-usage-requirements`
- `05-configurations-debug-release`
- `06-testing-with-ctest`
- `07-googletest-and-fetchcontent`
- `08-external-scripts-and-generated-files`
- `09-versioning-in-cmake`
- `10-windows-exe-dll-version-info`
- `11-install-export-package`
- `12-swig-and-python-interface-generation`
> Note: Chapter 13 is a practical guidance chapter and does not currently have a runnable sample.
---
## Conventions used in this guide
- **Default** means letting CMake choose the default generator for the current machine.
- **Ninja** means explicitly using the Ninja generator.
- **Visual Studio** means explicitly using `Visual Studio 17 2022`.
- Commands are shown from the **repository root**.
- Output paths may vary slightly by platform and generator.
- For **multi-config generators** such as Visual Studio, build outputs usually appear under configuration folders like `Debug/` or `Release/`.
---
## Table of contents
- [01 - First Project](#01---first-project)
- [02 - Build Systems and Generators](#02---build-systems-and-generators)
- [03 - Building and Loading a DLL](#03---building-and-loading-a-dll)
- [04 - Usage Requirements](#04---usage-requirements)
- [05 - Configurations: Debug, Release, and SimDebug](#05---configurations-debug-release-and-simdebug)
- [06 - Testing with CTest](#06---testing-with-ctest)
- [07 - GoogleTest and Local Archive Setup](#07---googletest-and-local-archive-setup)
- [08 - External Scripts and Generated Files](#08---external-scripts-and-generated-files)
- [09 - Versioning in CMake](#09---versioning-in-cmake)
- [10 - Windows EXE and DLL Version Info](#10---windows-exe-and-dll-version-info)
- [11 - Install, Export, and Package](#11---install-export-and-package)
- [12 - SWIG and Python Interface Generation](#12---swig-and-python-interface-generation)
- [General Notes](#general-notes)
---
# 01 - First Project
Source folder:

```text
examples/01-first-project
````
## Default
### Configure

```bash
cmake -S examples/01-first-project -B build/01-first-project
```
### Build

```bash
cmake --build build/01-first-project
```
### Run
#### Linux / simple single-config layout

```bash
./build/01-first-project/first_project
```
#### Windows / simple single-config layout

```powershell
.\build\01-first-project\first_project.exe
```
#### Visual Studio-style output layout

```powershell
.\build\01-first-project\Debug\first_project.exe
```
## Ninja
### Configure

```bash
cmake -S examples/01-first-project -B build/01-first-project-ninja -G Ninja
```
### Build

```bash
cmake --build build/01-first-project-ninja
```
### Run
#### Linux

```bash
./build/01-first-project-ninja/first_project
```
#### Windows

```powershell
.\build\01-first-project-ninja\first_project.exe
```
## Visual Studio
### Configure

```bash
cmake -S examples/01-first-project -B build/01-first-project-vs -G "Visual Studio 17 2022"
```
### Build Debug
```bash
cmake --build build/01-first-project-vs --config Debug
```
### Run Debug

```powershell
.\build\01-first-project-vs\Debug\first_project.exe
```
### Build Release

```bash
cmake --build build/01-first-project-vs --config Release
```
### Run Release

```powershell
.\build\01-first-project-vs\Release\first_project.exe
```
---
# 02 - Build Systems and Generators
Source folder:

```text
examples/02-build-systems-and-generators
```
## Default
### Configure

```bash
cmake -S examples/02-build-systems-and-generators -B build/02-default
```
### Build

```bash
cmake --build build/02-default
```
### Run
#### Linux / simple single-config layout

```bash
./build/02-default/generators_demo
```
#### Windows / simple single-config layout

```powershell
.\build\02-default\generators_demo.exe
```
#### Visual Studio-style output layout

```powershell
.\build\02-default\Debug\generators_demo.exe
```
## Ninja
### Configure Debug

```bash
cmake -S examples/02-build-systems-and-generators -B build/02-ninja-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
```
### Build Debug

```bash
cmake --build build/02-ninja-debug
```
### Run Debug
#### Linux

```bash
./build/02-ninja-debug/generators_demo
```
#### Windows

```powershell
.\build\02-ninja-debug\generators_demo.exe
```
### Configure Release

```bash
cmake -S examples/02-build-systems-and-generators -B build/02-ninja-release -G Ninja -DCMAKE_BUILD_TYPE=Release
```
### Build Release

```bash
cmake --build build/02-ninja-release
```
### Run Release
#### Linux

```bash
./build/02-ninja-release/generators_demo
```
#### Windows

```powershell
.\build\02-ninja-release\generators_demo.exe
```
## Visual Studio
### Configure

```bash
cmake -S examples/02-build-systems-and-generators -B build/02-vs2022 -G "Visual Studio 17 2022"
```
### Build Debug

```bash
cmake --build build/02-vs2022 --config Debug
```
### Run Debug

```powershell
.\build\02-vs2022\Debug\generators_demo.exe
```
### Build Release

```bash
cmake --build build/02-vs2022 --config Release
```
### Run Release

```powershell
.\build\02-vs2022\Release\generators_demo.exe
```
---
# 03 - Building and Loading a DLL
Source folder:

```text
examples/03-libraries
```
> This example is intentionally Windows-focused because it uses `LoadLibrary`, `GetProcAddress`, and `FreeLibrary`.
## Default
### Configure

```bash
cmake -S examples/03-libraries -B build/03-libraries
```
### Build

```bash
cmake --build build/03-libraries
```
### Run
#### Windows / simple single-config layout

```powershell
.\build\03-libraries\dll_loader.exe
```
#### Visual Studio-style output layout

```powershell
.\build\03-libraries\Debug\dll_loader.exe
```
## Ninja
### Configure

```bash
cmake -S examples/03-libraries -B build/03-libraries-ninja -G Ninja
```
### Build

```bash
cmake --build build/03-libraries-ninja
```
### Run

```powershell
.\build\03-libraries-ninja\dll_loader.exe
```
## Visual Studio
### Configure

```bash
cmake -S examples/03-libraries -B build/03-libraries-vs -G "Visual Studio 17 2022"
```
### Build Debug

```bash
cmake --build build/03-libraries-vs --config Debug
```
### Run Debug

```powershell
.\build\03-libraries-vs\Debug\dll_loader.exe
```
### Build Release

```bash
cmake --build build/03-libraries-vs --config Release
```
### Run Release

```powershell
.\build\03-libraries-vs\Release\dll_loader.exe
```
---
# 04 - Usage Requirements
Source folder:

```text
examples/04-usage-requirements
```
## Default
### Configure

```bash
cmake -S examples/04-usage-requirements -B build/04-usage-requirements
```
### Build

```bash
cmake --build build/04-usage-requirements
```
### Run
#### Linux / simple single-config layout

```bash
./build/04-usage-requirements/usage_requirements_demo
```
#### Windows / simple single-config layout

```powershell
.\build\04-usage-requirements\usage_requirements_demo.exe
```
#### Visual Studio-style output layout

```powershell
.\build\04-usage-requirements\Debug\usage_requirements_demo.exe
```
## Ninja
### Configure

```bash
cmake -S examples/04-usage-requirements -B build/04-usage-requirements-ninja -G Ninja
```
### Build

```bash
cmake --build build/04-usage-requirements-ninja
```
### Run
#### Linux

```bash
./build/04-usage-requirements-ninja/usage_requirements_demo
```
#### Windows

```powershell
.\build\04-usage-requirements-ninja\usage_requirements_demo.exe
```
## Visual Studio
### Configure

```bash
cmake -S examples/04-usage-requirements -B build/04-usage-requirements-vs -G "Visual Studio 17 2022"
```
### Build Debug

```bash
cmake --build build/04-usage-requirements-vs --config Debug
```
### Run Debug

```powershell
.\build\04-usage-requirements-vs\Debug\usage_requirements_demo.exe
```
### Build Release

```bash
cmake --build build/04-usage-requirements-vs --config Release
```
### Run Release

```powershell
.\build\04-usage-requirements-vs\Release\usage_requirements_demo.exe
```
---
# 05 - Configurations: Debug, Release, and SimDebug
Source folder:

```text
examples/05-configurations-debug-release
```
## Default
### Configure

```bash
cmake -S examples/05-configurations-debug-release -B build/05-configurations-debug-release
```
### Build

```bash
cmake --build build/05-configurations-debug-release
```
### Run
#### Linux / simple single-config layout

```bash
./build/05-configurations-debug-release/config_demo
```
#### Windows / simple single-config layout

```powershell
.\build\05-configurations-debug-release\config_demo.exe
```
#### Visual Studio-style output layout

```powershell
.\build\05-configurations-debug-release\Debug\config_demo.exe
```
## Ninja
### Configure Debug

```bash
cmake -S examples/05-configurations-debug-release -B build/05-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
```
### Build Debug

```bash
cmake --build build/05-debug
```
### Run Debug
#### Linux

```bash
./build/05-debug/config_demo
```
#### Windows

```powershell
.\build\05-debug\config_demo.exe
```
### Configure Release

```bash
cmake -S examples/05-configurations-debug-release -B build/05-release -G Ninja -DCMAKE_BUILD_TYPE=Release
```
### Build Release

```bash
cmake --build build/05-release
```
### Run Release
#### Linux

```bash
./build/05-release/config_demo
```
#### Windows

```powershell
.\build\05-release\config_demo.exe
```
### Configure SimDebug

```bash
cmake -S examples/05-configurations-debug-release -B build/05-simdebug -G Ninja -DCMAKE_BUILD_TYPE=SimDebug
```
### Build SimDebug

```bash
cmake --build build/05-simdebug
```
### Run SimDebug
#### Linux

```bash
./build/05-simdebug/config_demo
```
#### Windows

```powershell
.\build\05-simdebug\config_demo.exe
```
## Visual Studio
### Configure

```bash
cmake -S examples/05-configurations-debug-release -B build/05-vs -G "Visual Studio 17 2022"
```
### Build Debug

```bash
cmake --build build/05-vs --config Debug
```
### Run Debug

```powershell
.\build\05-vs\Debug\config_demo.exe
```
### Build Release

```bash
cmake --build build/05-vs --config Release
```
### Run Release

```powershell
.\build\05-vs\Release\config_demo.exe
```
### Build SimDebug

```bash
cmake --build build/05-vs --config SimDebug
```
### Run SimDebug

```powershell
.\build\05-vs\SimDebug\config_demo.exe
```
---
# 06 - Testing with CTest
Source folder:

```text
examples/06-testing-with-ctest
```
## Default
### Configure

```bash
cmake -S examples/06-testing-with-ctest -B build/06-testing-with-ctest
```
### Build

```bash
cmake --build build/06-testing-with-ctest
```
### Run test executable directly
#### Linux / simple single-config layout

```bash
./build/06-testing-with-ctest/core_math_tests
```
#### Windows / simple single-config layout

```powershell
.\build\06-testing-with-ctest\core_math_tests.exe
```
#### Visual Studio-style output layout

```powershell
.\build\06-testing-with-ctest\Debug\core_math_tests.exe
```
### Run through CTest

```bash
ctest --test-dir build/06-testing-with-ctest
```
### Run through CTest with output on failure
```bash
ctest --test-dir build/06-testing-with-ctest --output-on-failure
```
### Run specific test

```bash
ctest --test-dir build/06-testing-with-ctest -R core_math_tests
```
## Ninja
### Configure

```bash
cmake -S examples/06-testing-with-ctest -B build/06-testing-with-ctest-ninja -G Ninja
```
### Build

```bash
cmake --build build/06-testing-with-ctest-ninja
```
### Run test executable directly
#### Linux

```bash
./build/06-testing-with-ctest-ninja/core_math_tests
```
#### Windows

```powershell
.\build\06-testing-with-ctest-ninja\core_math_tests.exe
```
### Run through CTest

```bash
ctest --test-dir build/06-testing-with-ctest-ninja --output-on-failure
```
## Visual Studio
### Configure

```bash
cmake -S examples/06-testing-with-ctest -B build/06-testing-with-ctest-vs -G "Visual Studio 17 2022"
```
### Build Debug

```bash
cmake --build build/06-testing-with-ctest-vs --config Debug
```
### Run test executable directly

```powershell
.\build\06-testing-with-ctest-vs\Debug\core_math_tests.exe
```
### Run through CTest

```bash
ctest --test-dir build/06-testing-with-ctest-vs -C Debug --output-on-failure
```
---
# 07 - GoogleTest and Local Archive Setup
Source folder:

```text
examples/07-googletest-and-fetchcontent
```
This example uses a **local GoogleTest archive** instead of downloading from the network.
Expected local file:

```text
examples/07-googletest-and-fetchcontent/third_party/googletest-1.17.0.tar.gz
```
You can also override the archive path during configure with:

```bash
-DGTEST_ARCHIVE="full/path/to/googletest-1.17.0.tar.gz"
```
## Default
### Configure

```bash
cmake -S examples/07-googletest-and-fetchcontent -B build/07-googletest-and-fetchcontent
```
### Build

```bash
cmake --build build/07-googletest-and-fetchcontent
```
### Run test executable directly
#### Linux / simple single-config layout

```bash
./build/07-googletest-and-fetchcontent/core_math_gtests
```
#### Windows / simple single-config layout

```powershell
.\build\07-googletest-and-fetchcontent\core_math_gtests.exe
```
#### Visual Studio-style output layout

```powershell
.\build\07-googletest-and-fetchcontent\Debug\core_math_gtests.exe
```
### Run through CTest

```bash
ctest --test-dir build/07-googletest-and-fetchcontent --output-on-failure
```
## Ninja
### Configure

```bash
cmake -S examples/07-googletest-and-fetchcontent -B build/07-googletest-and-fetchcontent-ninja -G Ninja
```
### Build

```bash
cmake --build build/07-googletest-and-fetchcontent-ninja
```
### Run test executable directly
#### Linux

```bash
./build/07-googletest-and-fetchcontent-ninja/core_math_gtests
```
#### Windows

```powershell
.\build\07-googletest-and-fetchcontent-ninja\core_math_gtests.exe
```
### Run through CTest

```bash
ctest --test-dir build/07-googletest-and-fetchcontent-ninja --output-on-failure
```
## Visual Studio
### Configure

```bash
cmake -S examples/07-googletest-and-fetchcontent -B build/07-googletest-and-fetchcontent-vs -G "Visual Studio 17 2022"
```
### Build Debug

```bash
cmake --build build/07-googletest-and-fetchcontent-vs --config Debug
```
### Run test executable directly

```powershell
.\build\07-googletest-and-fetchcontent-vs\Debug\core_math_gtests.exe
```
### Run through CTest

```bash
ctest --test-dir build/07-googletest-and-fetchcontent-vs -C Debug --output-on-failure
```
---
# 08 - External Scripts and Generated Files
Source folder:

```text
examples/08-external-scripts-and-generated-files
```
## Default
### Configure

```bash
cmake -S examples/08-external-scripts-and-generated-files -B build/08-external-scripts-and-generated-files
```
### Build

```bash
cmake --build build/08-external-scripts-and-generated-files
```
### Run
#### Linux / simple single-config layout

```bash
./build/08-external-scripts-and-generated-files/generated_files_demo
```
#### Windows / simple single-config layout

```powershell
.\build\08-external-scripts-and-generated-files\generated_files_demo.exe
```
#### Visual Studio-style output layout

```powershell
.\build\08-external-scripts-and-generated-files\Debug\generated_files_demo.exe
```
## Ninja
### Configure

```bash
cmake -S examples/08-external-scripts-and-generated-files -B build/08-external-scripts-and-generated-files-ninja -G Ninja
```
### Build

```bash
cmake --build build/08-external-scripts-and-generated-files-ninja
```
### Run
#### Linux

```bash
./build/08-external-scripts-and-generated-files-ninja/generated_files_demo
```
#### Windows

```powershell
.\build\08-external-scripts-and-generated-files-ninja\generated_files_demo.exe
```
## Visual Studio
### Configure

```bash
cmake -S examples/08-external-scripts-and-generated-files -B build/08-external-scripts-and-generated-files-vs -G "Visual Studio 17 2022"
```
### Build Debug

```bash
cmake --build build/08-external-scripts-and-generated-files-vs --config Debug
```
### Run Debug

```powershell
.\build\08-external-scripts-and-generated-files-vs\Debug\generated_files_demo.exe
```
### Build Release

```bash
cmake --build build/08-external-scripts-and-generated-files-vs --config Release
```
### Run Release

```powershell
.\build\08-external-scripts-and-generated-files-vs\Release\generated_files_demo.exe
```
---
# 09 - Versioning in CMake
Source folder:

```text
examples/09-versioning-in-cmake
```
## Default
### Configure

```bash
cmake -S examples/09-versioning-in-cmake -B build/09-versioning-in-cmake
```
### Build

```bash
cmake --build build/09-versioning-in-cmake
```
### Run
#### Linux / simple single-config layout

```bash
./build/09-versioning-in-cmake/versioning_demo
```
#### Windows / simple single-config layout

```powershell
.\build\09-versioning-in-cmake\versioning_demo.exe
```
#### Visual Studio-style output layout

```powershell
.\build\09-versioning-in-cmake\Debug\versioning_demo.exe
```
## Ninja
### Configure

```bash
cmake -S examples/09-versioning-in-cmake -B build/09-versioning-in-cmake-ninja -G Ninja
```
### Build

```bash
cmake --build build/09-versioning-in-cmake-ninja
```
### Run
#### Linux

```bash
./build/09-versioning-in-cmake-ninja/versioning_demo
```
#### Windows

```powershell
.\build\09-versioning-in-cmake-ninja\versioning_demo.exe
```
### Bump patch

```bash
cmake --build build/09-versioning-in-cmake-ninja --target bump_patch
cmake -S examples/09-versioning-in-cmake -B build/09-versioning-in-cmake-ninja -G Ninja
cmake --build build/09-versioning-in-cmake-ninja
```
### Bump minor

```bash
cmake --build build/09-versioning-in-cmake-ninja --target bump_minor
cmake -S examples/09-versioning-in-cmake -B build/09-versioning-in-cmake-ninja -G Ninja
cmake --build build/09-versioning-in-cmake-ninja
```
### Bump major

```bash
cmake --build build/09-versioning-in-cmake-ninja --target bump_major
cmake -S examples/09-versioning-in-cmake -B build/09-versioning-in-cmake-ninja -G Ninja
cmake --build build/09-versioning-in-cmake-ninja
```
## Visual Studio
### Configure

```bash
cmake -S examples/09-versioning-in-cmake -B build/09-versioning-in-cmake-vs -G "Visual Studio 17 2022"
```
### Build Debug

```bash
cmake --build build/09-versioning-in-cmake-vs --config Debug
```
### Run Debug

```powershell
.\build\09-versioning-in-cmake-vs\Debug\versioning_demo.exe
```
### Build Release

```bash
cmake --build build/09-versioning-in-cmake-vs --config Release
```
### Run Release

```powershell
.\build\09-versioning-in-cmake-vs\Release\versioning_demo.exe
```
---
# 10 - Windows EXE and DLL Version Info
Source folder:
```text
examples/10-windows-exe-dll-version-info
```
> This example is Windows-focused because it uses Windows resource files (`.rc`) to stamp version metadata into the EXE and DLL.
## Ninja
### Configure
```bash
cmake -S examples/10-windows-exe-dll-version-info -B build/10-windows-exe-dll-version-info-ninja -G Ninja
```
### Build
```bash
cmake --build build/10-windows-exe-dll-version-info-ninja
```
### Run
```powershell
.\build\10-windows-exe-dll-version-info-ninja\version_info_app.exe
```
## Visual Studio
### Configure
```bash
cmake -S examples/10-windows-exe-dll-version-info -B build/10-windows-exe-dll-version-info-vs -G "Visual Studio 17 2022"
```
### Build Debug
```bash
cmake --build build/10-windows-exe-dll-version-info-vs --config Debug
```
### Run Debug
```powershell
.\build\10-windows-exe-dll-version-info-vs\Debug\version_info_app.exe
```
### Build Release
```bash
cmake --build build/10-windows-exe-dll-version-info-vs --config Release
```
### Run Release
```powershell
.\build\10-windows-exe-dll-version-info-vs\Release\version_info_app.exe
```
### Windows verification step
After building:
1. right-click `version_info_app.exe`
2. open **Properties**
3. inspect the **Details** tab
Do the same for `demo_dll.dll`.
---
# 11 - Install, Export, and Package
Source folder:
```text
examples/11-install-export-package
```
This example contains both:
* a **producer** project
* a **consumer** project under `consumer/`
The producer builds and installs a reusable package.
The consumer uses `find_package(...)` to consume the installed package.
## Producer - Ninja
### Configure
```bash
cmake -S examples/11-install-export-package -B build/11-install-export-package-ninja -G Ninja
```
### Build
```bash
cmake --build build/11-install-export-package-ninja
```
### Install
```bash
cmake --install build/11-install-export-package-ninja --prefix build/11-install-export-package-ninja/install
```
## Producer - Visual Studio
### Configure
```bash
cmake -S examples/11-install-export-package -B build/11-install-export-package-vs -G "Visual Studio 17 2022"
```
### Build Debug
```bash
cmake --build build/11-install-export-package-vs --config Debug
```
### Install Debug
```bash
cmake --install build/11-install-export-package-vs --config Debug --prefix build/11-install-export-package-vs/install
```
## Consumer - Ninja (explicit package config directory)
### Configure
```bash
cmake -S examples/11-install-export-package/consumer -B build/11-consumer-ninja -G Ninja -DTUTORIAL_MATH_CONFIG_DIR="build/11-install-export-package-ninja/install/lib/cmake/TutorialMath"
```
### Build
```bash
cmake --build build/11-consumer-ninja
```
### Run
#### Windows
```powershell
.\build\11-consumer-ninja\tutorial_math_consumer.exe
```
#### Linux
```bash
./build/11-consumer-ninja/tutorial_math_consumer
```
## Consumer - Visual Studio
### Configure
```bash
cmake -S examples/11-install-export-package/consumer -B build/11-consumer-vs -G "Visual Studio 17 2022" -DTUTORIAL_MATH_CONFIG_DIR="build/11-install-export-package-vs/install/lib/cmake/TutorialMath"
```
### Build Debug
```bash
cmake --build build/11-consumer-vs --config Debug
```
### Run Debug
```powershell
.\build\11-consumer-vs\Debug\tutorial_math_consumer.exe
```
## Package generation with CPack
### Ninja
```bash
cpack --config build/11-install-export-package-ninja/CPackConfig.cmake
```
### Visual Studio
```bash
cpack --config build/11-install-export-package-vs/CPackConfig.cmake -C Debug
```
---
# 12 - SWIG and Python Interface Generation
Source folder:
```text
examples/12-swig-and-python-interface-generation
```
> This example depends on external tools and environment setup. Read `examples/12-swig-and-python-interface-generation/README.md` before running it.
For this chapter, the most reliable path on Windows is:
* use **Ninja**
* configure as **Release**
* run the demo with the **same Python interpreter CMake found**
## Ninja
### Configure
```bash
cmake -S examples/12-swig-and-python-interface-generation -B build/12-swig-and-python-interface-generation-ninja -G Ninja -DCMAKE_BUILD_TYPE=Release -DSWIG_EXECUTABLE="C:/path/to/swig.exe"
```
### Build
```bash
cmake --build build/12-swig-and-python-interface-generation-ninja
```
### Run helper target
```bash
cmake --build build/12-swig-and-python-interface-generation-ninja --target run_demo
```
### Run Python demo directly
#### Windows PowerShell
```powershell
& "C:\Program Files\Python314\python.exe" .\build\12-swig-and-python-interface-generation-ninja\python\demo.py
```
#### Linux/macOS shell
```bash
python3 ./build/12-swig-and-python-interface-generation-ninja/python/demo.py
```
## Visual Studio
### Configure
```bash
cmake -S examples/12-swig-and-python-interface-generation -B build/12-swig-and-python-interface-generation-vs -G "Visual Studio 17 2022" -DSWIG_EXECUTABLE="C:/path/to/swig.exe"
```
### Build Release
```bash
cmake --build build/12-swig-and-python-interface-generation-vs --config Release
```
### Run demo
Use the same Python interpreter CMake found during configure.
---
# General Notes
## Cleaning a build tree
If a build tree becomes stale or messy, deleting it is a normal reset strategy.
Example:
```text
delete build/05-debug
reconfigure
rebuild
```
If you are using a newer CMake version, a fresh configure can also be useful:
```bash
cmake --fresh -S . -B build
```
## Useful reminders
### Single-config generators
Examples:
* Ninja
* Unix Makefiles
These usually use one build type per build folder.
### Multi-config generators
Examples:
* Visual Studio
* Ninja Multi-Config
These usually select the configuration at build time with `--config`.
## Suggested naming style for build folders
A clear naming pattern helps a lot:
```text
build/01-first-project
build/02-ninja-debug
build/03-libraries-vs
build/05-simdebug
build/07-googletest-and-fetchcontent-ninja
build/12-swig-and-python-interface-generation-ninja
```
## Suggested workflow
For each example:
1. configure into a dedicated build folder
2. build the example
3. run the executable or tests
4. compare behavior across generators or configurations when relevant
This keeps the tutorial clean and makes experiments easy to repeat.
## Common practical note
When a sample depends on external tools or runtimes:
* verify the tool exists first
* verify CMake found the expected executable
* keep the build tree disposable
* prefer a clean reset over fighting stale cache state too long
