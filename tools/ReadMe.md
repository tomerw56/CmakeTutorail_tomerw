## 1) `cmake/warnings.cmake`
### How to use it
In an example `CMakeLists.txt`:

```
cmake
include(${CMAKE_SOURCE_DIR}/cmake/warnings.cmake)
add_executable(my_demo main.cpp)
enable_project_warnings(my_demo)
```
This keeps warning level and “warnings as errors” target-local, which is the cleaner modern pattern. CMake documents `target_compile_options()` as target-specific, and `COMPILE_WARNING_AS_ERROR` as the dedicated target property when available. ([CMake][2])
---
## 2) `CMakePresets.json`
This version stays compatible with your `3.23` baseline by using preset schema version `4`.
### How to use it

```bash
cmake --preset ninja-debug
cmake --build --preset build-ninja-debug
ctest --preset test-ninja-debug
```
Presets are specifically intended to share common project-wide configure/build settings, and preset schema version `4` is the version added in CMake `3.23`. ([CMake][1])
---
## 3) `docs/clean-reset.md`
When a CMake build gets into a strange state, use one of these reset paths.
---
## Option A - Manual clean reset
This works on all tutorial-supported CMake versions.
### Windows cmd

```bat
rmdir /s /q build
cmake -S . -B build
cmake --build build
````
### PowerShell

```powershell
Remove-Item -Recurse -Force .\build
cmake -S . -B build
cmake --build build
```
### Linux/macOS shell

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```
---
## Option B - Fresh configure
If you are using CMake 3.24 or newer, you can use:

```bash
cmake --fresh -S . -B build
cmake --build build
```
---
## When to use a reset
A reset is a good idea when:
* package lookup changed
* generator choice changed
* a tool path changed
* cache values are stale
* a previous experiment left the build tree in a bad state
---
## Practical rule
If you are not sure whether the cache is still trustworthy:
* do a fresh configure
* or delete the build folder and start again

[1]: https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html "cmake-presets(7) — CMake 4.3.2 Documentation"
[2]: https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_WARNING_AS_ERROR.html "COMPILE_WARNING_AS_ERROR — CMake 4.3.2 Documentation"
