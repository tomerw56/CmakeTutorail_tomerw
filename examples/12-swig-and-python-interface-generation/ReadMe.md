# Chapter 12 Example - SWIG and Python Interface Generation
This example builds a small Python extension module from C++ code using:
- **SWIG**
- **Python 3**
- **CMake**
The example shows how to:
- wrap a small C++ API with SWIG
- build a Python-loadable native module
- import that module from Python
- keep the generated Python files and native extension in one output folder
---
## What this example needs
You need these tools installed and available:
- **CMake**
- **A working C++ compiler/toolchain**
- **Python 3**
- **SWIG**
For Windows, a practical setup is:
- Visual Studio 2022
- or Ninja + MSVC / Clang
- Python 3 installed normally
- SWIG extracted and available on `PATH`, or explicitly passed to CMake
---
## Folder layout
```text
12-swig-and-python-interface-generation/
├─ CMakeLists.txt
├─ README.md
├─ cpp/
│  ├─ math_utils.cpp
│  └─ math_utils.h
├─ swig/
│  └─ math.i
└─ python/
   └─ demo.py
````
* `cpp/` contains the native C++ code
* `swig/` contains the SWIG interface file
* `python/` contains the Python demo script
---
## Important practical note
This chapter has **two external prerequisites**:
1. **SWIG**
2. **Python 3**
That means setup matters more here than in a pure C++ example.
The most common bottleneck is **Python interpreter mismatch**:
the module may build successfully, but fail to import if you run it with a different Python interpreter than the one CMake found during configure.
So for this chapter, the most important habit is:
* configure CMake with the Python you want
* build the module against that Python
* run the demo with that exact same Python
---
## Python interpreter bottleneck
This is the main thing to watch.
If CMake finds one Python installation, but later you run:
```powershell
python demo.py
```
with a different Python from your `PATH`, the import may fail even though the build succeeded.
Typical symptom:
```text
ImportError: DLL load failed while importing _tutorial_math: The specified module could not be found.
```
In that situation, `_tutorial_math.pyd` usually exists, but one of its runtime dependencies does not match the Python interpreter being used.
### Practical rule
Use the **same Python** for:
* CMake configure
* module build
* running the demo
---
## How to verify which Python you are using
### Check the Python on your command line
```powershell
python -c "import sys; print(sys.executable); print(sys.version)"
```
### Recommended CMake diagnostic
Add these lines to `CMakeLists.txt` after `find_package(Python3 ...)` if you want the configure step to print what it found:
```cmake
message(STATUS "Python3_EXECUTABLE = ${Python3_EXECUTABLE}")
message(STATUS "Python3_VERSION    = ${Python3_VERSION}")
```
Then compare that output with the interpreter you actually use to run the script.
---
## How to point CMake at SWIG explicitly
If `swig.exe` is not on your `PATH`, pass it directly to CMake with:
```powershell
-DSWIG_EXECUTABLE="C:/path/to/swig.exe"
```
Example:
```powershell
cmake -S examples/12-swig-and-python-interface-generation `
      -B build/12-swig-and-python-interface-generation-ninja `
      -G Ninja `
      -DCMAKE_BUILD_TYPE=Release `
      -DSWIG_EXECUTABLE="C:/tools/swigwin-4.3.1/swig.exe"
```
Using forward slashes is recommended.
---
## Recommended Windows path for this chapter
For Windows, the smoothest path is:
* use **Ninja**
* configure as **Release**
* run the demo with the **exact Python executable CMake found**
This chapter can work with Visual Studio too, but Ninja is the simpler first path.
---
## Recommended Windows setup flow
### 1. Verify SWIG
```powershell
swig -version
```
If that fails, either:
* add SWIG to `PATH`
* or pass `-DSWIG_EXECUTABLE=...` to CMake
### 2. Verify Python
```powershell
python -c "import sys; print(sys.executable); print(sys.version)"
```
### 3. Configure in Release with Ninja
```powershell
cmake -S examples/12-swig-and-python-interface-generation `
      -B build/12-swig-and-python-interface-generation-ninja `
      -G Ninja `
      -DCMAKE_BUILD_TYPE=Release `
      -DSWIG_EXECUTABLE="C:/path/to/swig.exe"
```
### 4. Build
```powershell
cmake --build build/12-swig-and-python-interface-generation-ninja
```
### 5. Run using the exact Python CMake used
Example:
```powershell
& "C:\Program Files\Python314\python.exe" .\build\12-swig-and-python-interface-generation-ninja\python\demo.py
```
---
## Configure and build
## Ninja
### Configure
```bash
cmake -S examples/12-swig-and-python-interface-generation \
      -B build/12-swig-and-python-interface-generation-ninja \
      -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DSWIG_EXECUTABLE="C:/path/to/swig.exe"
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
---
## Visual Studio
Visual Studio can be used, but the example is best understood first through Ninja.
If you do use Visual Studio:
### Configure
```bash
cmake -S examples/12-swig-and-python-interface-generation \
      -B build/12-swig-and-python-interface-generation-vs \
      -G "Visual Studio 17 2022" \
      -DSWIG_EXECUTABLE="C:/path/to/swig.exe"
```
### Build Release
```bash
cmake --build build/12-swig-and-python-interface-generation-vs --config Release
```
Then run the demo with the same Python CMake found.
---
## Expected output
```text
10 + 5 = 15
10 * 5 = 50
```
---
## Troubleshooting
### CMake cannot find SWIG
Check:
```powershell
swig -version
```
If that fails:
* SWIG is not installed
* or it is not on `PATH`
Use:
```powershell
-DSWIG_EXECUTABLE="C:/path/to/swig.exe"
```
during configure.
---
### The build succeeds but Python import fails
If you see something like:
```text
ImportError: DLL load failed while importing _tutorial_math: The specified module could not be found.
```
then the most likely issue is:
* the module was built against one Python installation
* but you ran the script with another Python
Fix:
* run the script with the exact `Python3_EXECUTABLE` CMake found
---
### Debug build problems on Windows
For this chapter, prefer:
* `Release`
* or `RelWithDebInfo`
A plain `Debug` build can be more fragile with normal Windows Python installations.
For the tutorial, `Release` is the recommended default.
---
### The generated Python files exist, but import still fails
Check that the output folder contains both:
* `tutorial_math.py`
* `_tutorial_math.pyd`
Also make sure you run the script from the expected generated output location, or use the exact path shown above.
---
### Clean reset
If the build gets into a bad state, delete the build folder and reconfigure.
#### PowerShell
```powershell
Remove-Item -Recurse -Force .\build\12-swig-and-python-interface-generation-ninja
```
Then configure and build again.
---
## Practical recommendation
For this chapter, the most reliable first workflow is:
1. verify SWIG
2. verify Python
3. configure with Ninja in Release
4. build
5. run the demo with the exact Python CMake found
