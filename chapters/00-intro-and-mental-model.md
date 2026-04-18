
# Chapter 00 - Introduction and Mental Model
This chapter is about building the right mental model before writing any real `CMakeLists.txt`.
A lot of CMake confusion comes from starting with commands before understanding what CMake is doing.
So before we create executables, libraries, tests, or packages, we want a small set of ideas that will make the rest of the tutorial much easier to follow.
---
## Why this matters
Many beginners treat CMake like a compiler command with extra syntax.
It is not.
CMake is a **build system generator**.
You describe your project in CMake files, and CMake generates files for another tool that actually performs the build.
That idea affects almost everything:
- why we have a configure step
- why Ninja and Visual Studio both work
- why build directories exist
- why some variables are known during configure time and others matter during build time
- why generated files and custom commands need careful dependency handling
If this mental model is clear, the rest of the tutorial becomes much more predictable.
---
## The shortest useful definition
CMake is a tool that:
1. reads your project description
2. configures the build
3. generates files for a backend build system
4. lets that backend compile and link the code
Examples of backend generators:
- Ninja
- Unix Makefiles
- Visual Studio
So when you run:
```bash
cmake -S . -B build
````
you are **not compiling yet**.
You are asking CMake to read the project and generate a build tree.
When you run:
```bash
cmake --build build
```
you are asking CMake to invoke the generated backend build system.
---
## CMake is not the compiler
This is probably the single most important sentence in the chapter:
> CMake is not the compiler, and usually not the build tool either.
For example:
* with **Ninja**, CMake generates Ninja build files, and Ninja drives the compiler
* with **Visual Studio**, CMake generates Visual Studio project/solution data, and MSBuild or Visual Studio drives the compiler
* with **Unix Makefiles**, CMake generates Makefiles, and `make` drives the compiler
So CMake sits one level above the actual build backend.
---
## The three phases: configure, build, install
A clean way to think about CMake is as three main phases.
## 1. Configure
In the configure phase, CMake:
* reads `CMakeLists.txt`
* evaluates variables and logic
* checks tools and compilers
* finds packages
* prepares target definitions
* generates backend build files
Typical command:
```bash
cmake -S . -B build
```
This phase creates or updates the build directory.
---
## 2. Build
In the build phase, the backend tool actually does the work:
* compile source files
* link executables
* link libraries
* run custom build steps
* generate outputs that belong to the build graph
Typical command:
```bash
cmake --build build
```
You can think of this as:
“Use the build files generated in `build/` and perform the compilation.”
---
## 3. Install
The install phase copies selected outputs into an install layout.
Typical command:
```bash
cmake --install build
```
This is how a project can place:
* executables
* DLLs / shared libraries
* static libraries
* headers
* package config files
into a structured install prefix.
We will cover install properly later, but it is useful to know from the start that configure, build, and install are different things.
---
## Source tree vs build tree
Another essential idea:
## Source tree
The source tree contains:
* source code
* headers
* `CMakeLists.txt`
* markdown docs
* scripts
* assets that belong to the project
Example:
```text
my_project/
├─ CMakeLists.txt
├─ src/
├─ include/
└─ tests/
```
## Build tree
The build tree contains generated build artifacts and intermediate files:
* object files
* generated backend files
* caches
* compiled outputs
* generated headers
* temporary files
Example:
```text
my_project/
├─ CMakeLists.txt
├─ src/
├─ include/
├─ tests/
└─ build/
```
A common and recommended workflow is an **out-of-source build**, where all generated files go into a separate build directory.
This keeps the source tree clean and makes it easy to:
* delete and rebuild from scratch
* have multiple build configurations
* avoid polluting the repo with generated files
---
## Why out-of-source builds matter
We will strongly prefer this pattern in the tutorial:
```bash
cmake -S . -B build
cmake --build build
```
Why this is good:
* easy cleanup: delete `build/`
* easy to keep multiple build folders:
  * `build-ninja-debug/`
  * `build-vs2022/`
  * `build-clang-release/`
* keeps generated files out of source control
* makes examples easier to reason about
If you remember only one workflow pattern from this chapter, remember that one.
---
## What a generator is
A **generator** tells CMake what kind of build files to create.
Examples:
* `Ninja`
* `Unix Makefiles`
* `Visual Studio 17 2022`
Example:
```bash
cmake -S . -B build -G Ninja
```
That says:
* source dir is `.`
* build dir is `build`
* generate Ninja-based build files
Or:
```bash
cmake -S . -B build -G "Visual Studio 17 2022"
```
That says:
* generate Visual Studio build files instead
The project description can stay mostly the same while the backend changes.
This is one of the strongest reasons people use CMake.
---
## Single-config vs multi-config generators
This is a very important distinction.
## Single-config generators
Examples:
* Ninja
* Unix Makefiles
These usually use one configuration per build directory.
For example, you configure once for Debug:
```bash
cmake -S . -B build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
```
And separately for Release:
```bash
cmake -S . -B build-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```
Each build tree is tied to one main configuration.
---
## Multi-config generators
Examples:
* Visual Studio
* some Xcode flows
These generators can manage multiple configurations in the same build tree.
Typical Visual Studio flow:
```bash
cmake -S . -B build-vs -G "Visual Studio 17 2022"
cmake --build build-vs --config Debug
cmake --build build-vs --config Release
```
That means:
* the configuration is often selected at build time
* the same build tree can hold multiple configs
This difference explains a lot of “why does this command work here but not there?” moments in real projects.
---
## What a target is
Modern CMake is target-oriented.
A **target** is a named build entity such as:
* an executable
* a static library
* a shared library
* an interface library
* a custom target
Examples:
* `my_app`
* `math_lib`
* `unit_tests`
Targets are the main units we define and connect.
Instead of thinking:
* “set lots of global flags everywhere”
modern CMake encourages thinking:
* “define a target”
* “attach properties to it”
* “link it to other targets”
This becomes much clearer once we start writing actual code, but the idea belongs here.
---
## What targets carry
A target can carry information such as:
* source files
* include directories
* compile definitions
* compile options
* compile features
* linked libraries
* usage requirements
This is why target-based CMake is more maintainable than older global-variable-heavy styles.
You can often read a CMake project by asking:
1. what are the targets?
2. how are they connected?
3. what does each target expose to others?
That is a very useful habit.
---
## Configure-time thinking vs build-time thinking
This is another idea that saves a lot of pain.
Some work happens at **configure time**:
* evaluating `if()` logic
* deciding options
* locating tools
* generating some files with `configure_file()`
* creating the build graph
Other work happens at **build time**:
* compiling source files
* linking targets
* running commands attached with custom build rules
* generating outputs that belong to the build graph
A common beginner mistake is mixing these two worlds.
Examples:
* expecting a build-generated file to exist during initial configure
* using configure-time logic for something that should be tracked as a build dependency
* forgetting that generated outputs need declared dependencies
We will revisit this when we get to scripts and generated files.
For now, just keep the separation in mind:
* configure time defines the build
* build time executes the build
---
## Cache variables and normal variables
You do not need full detail yet, but you should know the difference exists.
CMake has:
* normal variables
* cache variables
A cache variable is stored in the build tree and survives reconfigure runs.
This is why settings can “stick” between runs.
Example:
```bash
cmake -S . -B build -DMY_OPTION=ON
```
That `-D` value goes into the CMake cache in the build directory.
This is useful, but it also explains why build trees sometimes behave in surprising ways after earlier experiments.
When in doubt:
* inspect the cache
* or delete the build directory and reconfigure
That is one reason out-of-source builds are so practical.
---
## Why deleting the build folder is normal
In many ecosystems, deleting the build directory feels drastic.
In CMake projects, it is often perfectly normal.
Because the build tree is generated data, removing it is a safe and common reset mechanism.
Typical cleanup strategy:
```text
delete build/
re-run configure
re-run build
```
This is not a hack.
It is often the cleanest way to recover from:
* stale cache values
* changed generators
* compiler switches that no longer match
* experiments that left the build tree in a weird state
---
## A minimal mental workflow
Here is the workflow we want readers to internalize:
### 1. Write or update project files
For example:
* `CMakeLists.txt`
* source files
* headers
### 2. Configure
```bash
cmake -S . -B build
```
### 3. Build
```bash
cmake --build build
```
### 4. Run
Run the produced executable or tests.
### 5. Reconfigure when project structure changes
If you add targets, options, or files that affect the build graph, run configure again.
In practice, many IDEs help automate this, but the underlying model is still the same.
---
## Example command patterns
These are not the only commands we will use, but they are the core ones.
### Configure with default generator
```bash
cmake -S . -B build
```
### Configure with Ninja
```bash
cmake -S . -B build -G Ninja
```
### Configure Debug for a single-config generator
```bash
cmake -S . -B build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
```
### Build
```bash
cmake --build build
```
### Build Release with a multi-config generator
```bash
cmake --build build-vs --config Release
```
### Install
```bash
cmake --install build
```
### Run tests
```bash
ctest --test-dir build
```
We will explain each of these in context later.
---
## Common beginner mistakes
### 1. Mixing source files and build outputs in the same directory
This creates clutter and confusion.
Prefer out-of-source builds.
### 2. Forgetting that configure and build are different steps
Running configure does not compile.
Running build does not redefine the project structure.
### 3. Treating CMake like a compiler invocation
CMake defines and generates the build.
The backend and compiler do the compilation.
### 4. Not understanding generator differences
Visual Studio and Ninja do not behave exactly the same.
Especially around configuration handling.
### 5. Assuming the cache will automatically forget old settings
It often will not.
Sometimes the build tree needs to be cleaned or recreated.
### 6. Thinking only in terms of files, not targets
Modern CMake is easier when you think in terms of named targets and their relationships.
---
## This is enough for now
At this stage, you do not need to memorize every command or variable.
The key ideas to keep are:
* CMake is a build system generator
* configure and build are different phases
* source tree and build tree should be separate
* the generator controls what backend build files are created
* single-config and multi-config generators behave differently
* modern CMake is target-oriented
* deleting the build tree is a normal reset strategy
If those ideas are clear, the next chapter will feel much more natural.
---
## Takeaways
Before we write real CMake code, we already know the shape of the system:
* the project lives in the source tree
* CMake configures and generates a build tree
* a chosen backend builds the code
* targets are the main building blocks
* different generators lead to different workflows
* clean build directories make experiments safer and easier
That is the foundation for everything else in this tutorial.
---
## Next chapter
In the next chapter, we will create the smallest useful CMake project:
* one executable
* one `CMakeLists.txt`
* one source file
* one configure step
* one build step
That will give us our first complete runnable example.
```
