# Chapter 02 - Build Systems and Generators
In the previous chapter we created our first working CMake project.
Now we focus on one of the most important practical CMake topics:
**generators**.
This chapter explains why the same `CMakeLists.txt` can drive different build workflows, and why commands that feel natural with Ninja are sometimes slightly different with Visual Studio.
If Chapter 01 was about creating a first target, this chapter is about understanding the environment that target lives in.
---
## Why this matters
A lot of confusion around CMake comes from not knowing which part is CMake and which part is the backend build system.
When people say things like:
- "it works with Ninja but not with Visual Studio"
- "why is Debug handled differently here?"
- "why is my executable in a different folder?"
- "why do I need `--config Release` in one case but not another?"
they are usually hitting generator differences.
Once this clicks, many CMake workflows become much easier to reason about.
---
## What we will learn
In this chapter we will cover:
- what a generator is
- how to pick one
- the difference between single-config and multi-config generators
- why output locations can differ
- why Debug and Release are handled differently
- how to run the same simple project with different generators
We are still using a very small project on purpose.
The concepts are the important part here.
---
## What a generator is
A **generator** tells CMake what kind of build files to create.
CMake does not compile your source files directly.
Instead, it generates build instructions for another tool.
Examples of generators include:
- Ninja
- Unix Makefiles
- Visual Studio 17 2022
That means the same project description can be used to generate:
- Ninja build files on one machine
- Visual Studio project files on another
- Makefiles somewhere else
So the generator is the bridge between:
- your `CMakeLists.txt`
- the actual build backend
---
## The same project, different backends
Suppose we have this simple project:
```cmake
cmake_minimum_required(VERSION 3.23)
project(FirstProject LANGUAGES CXX)
add_executable(first_project main.cpp)
````
That file does not say:
* "use Ninja only"
* "use Visual Studio only"
* "use MSBuild only"
Instead, it describes the project in a more portable way.
Then **you choose the generator at configure time**.
That is one of the central ideas of CMake.
---
## Generator choice happens during configure
You choose the generator when you run the configure command.
### Example: default generator
```bash
cmake -S examples/01-first-project -B build/01-default
```
CMake will pick a default generator based on the platform and environment.
### Example: Ninja
```bash
cmake -S examples/01-first-project -B build/01-ninja -G Ninja
```
### Example: Visual Studio 2022
```bash
cmake -S examples/01-first-project -B build/01-vs2022 -G "Visual Studio 17 2022"
```
That generator choice becomes part of the build tree.
This is important:
a build directory is not just "some outputs folder".
It is tied to a particular generator and configuration model.
---
## One build folder per generator is a good habit
Because the generator is baked into the build tree, it is a good idea to keep different generators in different build folders.
Good examples:
```text
build/01-ninja/
build/01-vs2022/
build/01-make/
```
Bad example:
```text
build/
```
and then reusing it for different generators.
If you configured a build directory once with Visual Studio and later try to use the same build directory with Ninja, you are likely to get errors or confusing behavior.
A clean rule is:
* one build directory
* one generator
* one clear purpose
---
## Single-config vs multi-config generators
This is the most important distinction in this chapter.
### Single-config generators
Typical examples:
* Ninja
* Unix Makefiles
These usually represent one main build configuration per build directory.
For example, if you configure with:
```bash
cmake -S examples/01-first-project -B build/01-ninja-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
```
that build folder is effectively a Debug build tree.
If you want Release too, you usually create another build folder:
```bash
cmake -S examples/01-first-project -B build/01-ninja-release -G Ninja -DCMAKE_BUILD_TYPE=Release
```
So with single-config generators, configuration is usually chosen at configure time.
---
### Multi-config generators
Typical examples:
* Visual Studio
* some Xcode workflows
These can manage multiple configurations in the same build tree.
For example:
```bash
cmake -S examples/01-first-project -B build/01-vs2022 -G "Visual Studio 17 2022"
```
Then later you choose the configuration at build time:
```bash
cmake --build build/01-vs2022 --config Debug
cmake --build build/01-vs2022 --config Release
```
So with multi-config generators, configuration is often chosen at build time.
That is why Visual Studio workflows often look different from Ninja workflows.
---
## Why `CMAKE_BUILD_TYPE` is not universal
A very common beginner mistake is assuming this always controls build configuration:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```
That works naturally for many single-config generators.
But for multi-config generators like Visual Studio, the configuration is typically selected later with:
```bash
cmake --build build --config Release
```
So a useful rule is:
* **single-config**: `CMAKE_BUILD_TYPE` usually matters
* **multi-config**: `--config Debug/Release` usually matters
This one point explains a lot of confusing first experiences with CMake.
---
## Output directories can differ
Another thing that surprises people is executable location.
With a single-config generator like Ninja, a simple executable may appear directly under the build folder:
### Linux
```bash
./build/01-ninja-debug/first_project
```
### Windows
```powershell
.\build\01-ninja-debug\first_project.exe
```
With Visual Studio, you often get configuration-specific subfolders:
```powershell
.\build\01-vs2022\Debug\first_project.exe
```
or
```powershell
.\build\01-vs2022\Release\first_project.exe
```
So when a program "built successfully" but you cannot find it, check:
* which generator you used
* which configuration you built
* where that generator typically places outputs
---
## A practical comparison
We will compare the same tiny project under two workflows:
* Ninja
* Visual Studio 2022
The source files are still the same as in Chapter 01.
---
## Workflow A - Ninja
### Configure Debug
```bash
cmake -S examples/01-first-project -B build/02-ninja-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
```
### Build
```bash
cmake --build build/02-ninja-debug
```
### Run
#### Linux
```bash
./build/02-ninja-debug/first_project
```
#### Windows
```powershell
.\build\02-ninja-debug\first_project.exe
```
### Configure Release
```bash
cmake -S examples/01-first-project -B build/02-ninja-release -G Ninja -DCMAKE_BUILD_TYPE=Release
```
### Build Release
```bash
cmake --build build/02-ninja-release
```
### Run Release
#### Linux
```bash
./build/02-ninja-release/first_project
```
#### Windows
```powershell
.\build\02-ninja-release\first_project.exe
```
Notice the pattern:
* separate build folders
* each folder has one main configuration
---
## Workflow B - Visual Studio 2022
### Configure once
```bash
cmake -S examples/01-first-project -B build/02-vs2022 -G "Visual Studio 17 2022"
```
### Build Debug
```bash
cmake --build build/02-vs2022 --config Debug
```
### Run Debug
```powershell
.\build\02-vs2022\Debug\first_project.exe
```
### Build Release
```bash
cmake --build build/02-vs2022 --config Release
```
### Run Release
```powershell
.\build\02-vs2022\Release\first_project.exe
```
Notice the pattern:
* one build folder
* multiple configurations selected later
---
## Same source, different workflow
This is the key lesson.
The source project stays the same:
```cmake
cmake_minimum_required(VERSION 3.23)
project(FirstProject LANGUAGES CXX)
add_executable(first_project main.cpp)
```
What changes is:
* the generator
* the build tree
* the configuration workflow
* sometimes the output layout
That is exactly what CMake is for:
keep the project description as stable as possible while allowing different backend workflows.
---
## How to see available generators
A useful command is:
```bash
cmake --help
```
This usually prints general help and includes available generators on the current machine.
That is often the fastest way to answer:
* "what generators are available here?"
* "what exact Visual Studio generator string should I use?"
* "is Ninja installed and visible to CMake?"
---
## Choosing a generator in practice
A simple practical rule set is:
### Use Ninja when:
* you want a fast command-line workflow
* you want simple, direct builds
* you are comfortable with explicit build folders
* you want predictable single-config behavior
### Use Visual Studio when:
* your team works mainly in Visual Studio
* you want a familiar Windows IDE workflow
* you want multi-config behavior in one build tree
* you want easy Debug/Release switching without separate configure trees
Neither is "the one true answer".
The point is to understand the workflow each one implies.
---
## Why teams standardize this
Many teams choose one preferred generator for day-to-day work because it reduces friction.
Examples:
* "Everyone uses Ninja locally"
* "Windows developers use Visual Studio generator"
* "CI uses Ninja"
* "Release builds use a fixed generator and toolchain"
That kind of standardization helps because generator differences are real, and different defaults can make troubleshooting harder.
---
## Common mistakes
### 1. Reusing the same build folder with different generators
This is one of the most common mistakes.
If you want a different generator, create a different build folder.
### 2. Using `CMAKE_BUILD_TYPE` with Visual Studio and expecting that alone to control everything
For Visual Studio, `--config` is usually the key control at build time.
### 3. Forgetting which configuration was built
Especially with multi-config generators, Debug and Release can both exist in the same tree.
### 4. Looking for the executable in the wrong place
Different generators often place outputs differently.
### 5. Assuming the default generator is the same on every machine
It is not.
Defaults depend on platform and environment.
---
## This is enough for now
At this point, the key things to remember are:
* the generator is chosen during configure
* the build directory is tied to that generator
* single-config and multi-config generators behave differently
* Ninja and Visual Studio often require slightly different command patterns
* output locations can differ depending on generator and configuration
That is enough to make later chapters much less confusing.
---
## Takeaways
You should now be comfortable with these ideas:
* CMake describes the project
* the generator decides what kind of backend files get created
* build trees should usually be separated by generator
* single-config generators usually choose Debug/Release at configure time
* multi-config generators usually choose Debug/Release at build time
This is one of the most practical parts of understanding CMake.
---
## Next chapter
In the next chapter we will move from a single executable to multiple targets.
We will introduce:
* static libraries
* shared libraries
* linking an executable to a library
That is where target-based CMake starts to become much more interesting.
```
```
