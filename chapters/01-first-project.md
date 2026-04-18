---
# Chapter 01 - First Project
In this chapter we create the smallest useful CMake project:
- one executable
- one source file
- one `CMakeLists.txt`
- one configure step
- one build step
- one run step
The goal is not to show a clever example.
The goal is to make the first complete CMake workflow feel simple and predictable.
---
## Why this matters
This is the first point where the mental model from Chapter 00 becomes concrete.
We will:
- describe a project with CMake
- generate a build tree
- build the executable
- run the program
That full loop is the foundation for everything that comes later.
---
## What we will build
We will build a very small console application that prints a message.
Source file:
- `main.cpp`
CMake file:
- `CMakeLists.txt`
Output:
- an executable called `first_project`
---
## Folder layout
Example folder:
```text
examples/
└─ 01-first-project/
   ├─ CMakeLists.txt
   └─ main.cpp
````
---
## The code
## `CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 3.23)
project(FirstProject LANGUAGES CXX)
add_executable(first_project main.cpp)
```
## `main.cpp`
```cpp
#include <iostream>
int main()
{
    std::cout << "Hello from the first CMake project!\n";
    return 0;
}
```
---
## Line by line
## `cmake_minimum_required(VERSION 3.23)`
This tells CMake the minimum version required to process this project.
Why it matters:
* it sets the minimum supported CMake version
* it helps make behavior more predictable
* it signals what feature level the project expects
For this tutorial we will use a modern baseline.
---
## `project(FirstProject LANGUAGES CXX)`
This defines the project and tells CMake that this project uses C++.
At this point CMake starts setting up project-related variables and compiler handling for the requested language.
Important note:
* this does not build anything yet
* it defines project-level metadata and enables language support
---
## `add_executable(first_project main.cpp)`
This creates an executable target named `first_project`.
This line tells CMake:
* create an executable target
* use `main.cpp` as a source file
This is our first real target.
---
## What target name means here
The target name is:
```text
first_project
```
That name is important because it is how CMake refers to the executable internally.
Later, when we add:
* libraries
* include directories
* compile features
* tests
we will attach those things to targets by name.
---
## Configure the project
Open a terminal in the repository root and run:
```bash
cmake -S examples/01-first-project -B build/01-first-project
```
What this means:
* `-S examples/01-first-project` tells CMake where the source directory is
* `-B build/01-first-project` tells CMake where to generate the build tree
This creates a separate build directory for this example.
That is the pattern we want throughout the tutorial:
* keep source separate from generated output
* keep each example easy to build and easy to delete
---
## Build the project
After configure succeeds, build it with:
```bash
cmake --build build/01-first-project
```
This tells CMake to invoke the generated backend build system inside that build directory.
Depending on your generator and platform, this step will:
* compile `main.cpp`
* link the executable
* place the executable somewhere inside the build tree
---
## Run the executable
How you run the executable depends on the generator and platform.
### On Linux or with single-config generators
A common pattern is:
```bash
./build/01-first-project/first_project
```
### On Windows with a Ninja-style single-config build
A common pattern is:
```powershell
.\build\01-first-project\first_project.exe
```
### On Windows with Visual Studio
The executable is often under a configuration subdirectory such as `Debug` or `Release`:
```powershell
.\build\01-first-project\Debug\first_project.exe
```
or
```powershell
.\build\01-first-project\Release\first_project.exe
```
---
## Configure and build with Ninja
If you want to be explicit about the generator:
```bash
cmake -S examples/01-first-project -B build/01-first-project-ninja -G Ninja
cmake --build build/01-first-project-ninja
```
Run it with:
### Linux
```bash
./build/01-first-project-ninja/first_project
```
### Windows
```powershell
.\build\01-first-project-ninja\first_project.exe
```
---
## Configure and build with Visual Studio 2022
```bash
cmake -S examples/01-first-project -B build/01-first-project-vs -G "Visual Studio 17 2022"
cmake --build build/01-first-project-vs --config Debug
```
Run it with:
```powershell
.\build\01-first-project-vs\Debug\first_project.exe
```
You can also build Release:
```bash
cmake --build build/01-first-project-vs --config Release
```
and then run:
```powershell
.\build\01-first-project-vs\Release\first_project.exe
```
---
## Expected output
When you run the program, you should see:
```text
Hello from the first CMake project!
```
---
## What to notice
There are a few important things to notice in this tiny example.
### 1. The source tree stays clean
We did not generate files next to `main.cpp`.
### 2. Configure and build are separate
We first generated the build system, then built the code.
### 3. The target is named explicitly
The executable is created by target name, not by some global compiler command.
### 4. Output paths can vary by generator
Especially on Windows and with Visual Studio, outputs often appear under config-specific folders.
---
## Common mistakes
### 1. Running `cmake --build` before configure
You need the configure step first.
### 2. Running from the wrong directory
Be clear about the source path and build path.
### 3. Assuming output paths are identical for all generators
They are not.
Visual Studio often adds `Debug/` or `Release/`.
### 4. Mixing source and build files
Do not create outputs inside the example source folder.
### 5. Forgetting that the target name is not always the same as the project name
In this example:
* project name: `FirstProject`
* executable target name: `first_project`
They are related, but not required to be identical.
---
## This is enough for now
At this stage, the main things to remember are:
* a CMake project begins with `cmake_minimum_required()` and `project()`
* `add_executable()` creates a target
* configure and build are separate steps
* the executable is generated into the build tree
* generator choice affects where outputs appear
That is enough for the first complete loop.
---
## Takeaways
We now have our first real CMake project.
We learned how to:
* define a project
* define an executable target
* configure into a separate build directory
* build the target
* run the output
This is the smallest working pattern that the rest of the tutorial will extend.
---
## Next chapter
In the next chapter we will stay with a small project, but focus on generator choice and workflow differences:
* Ninja
* Visual Studio
* single-config vs multi-config behavior
* how that affects configure, build, and run commands
