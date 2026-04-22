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
# General Notes
## Cleaning a build tree
If a build tree becomes stale or messy, deleting it is a normal reset strategy.
Example:
```text
delete build/05-debug
reconfigure
rebuild
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
```
## Suggested workflow
For each example:
1. configure into a dedicated build folder
2. build the example
3. run the executable or tests
4. compare behavior across generators or configurations when relevant
This keeps the tutorial clean and makes experiments easy to repeat.