# CMake Tutorial
A practical, code-first CMake tutorial built around small runnable examples.
This repository is meant for developers who want to learn CMake in a way that matches real project work:
executables, libraries, generators, testing, dependencies, external scripts, versioning, and packaging.
The goal is not just to explain CMake syntax.  
The goal is to build a clear mental model and then apply it to realistic examples.
---
## What this repository is
This repo is a guided tutorial for learning modern CMake step by step.
Each chapter has:
- a focused topic
- a matching runnable example
- a markdown explanation
- practical commands
- common mistakes and notes
This repository is intentionally markdown-first and code-oriented.  
It is meant to feel like engineering documentation, not presentation material.
---
## Who this is for
This tutorial is aimed at:
- C and C++ developers new to CMake
- developers who already use CMake but want a cleaner mental model
- teams that want practical examples for everyday workflows
- Windows developers using Visual Studio
- developers using Ninja or other command-line generators
A basic knowledge of C++ and compiling code is assumed.
---
## What you will learn
By working through this repo, you will learn how to:
- configure and build a project with CMake
- understand the difference between source trees and build trees
- choose and use generators such as Ninja and Visual Studio
- create executables and libraries
- link targets correctly
- use include directories, compile features, and usage requirements
- work with Debug and Release configurations
- add tests with CTest
- integrate GoogleTest
- fetch dependencies from the network
- run external scripts as part of the build
- generate files during configuration or build
- manage project version information
- stamp version info into Windows EXE and DLL files
- install, export, and package projects
---
## How to read this repo
The recommended flow is:
1. Read the chapter markdown
2. Open the matching example folder
3. Configure and build the example yourself
4. Compare the commands and output with the explanation
5. Move to the next chapter
Each chapter is meant to stand on its own, but they are ordered to build up the mental model gradually.
If you are already familiar with the basics, you can jump directly to specific topics such as:
- GoogleTest
- FetchContent
- custom commands and external scripts
- Windows version resources

## Project helpers
This repo includes a few reusable practical helpers:
- `tools/cmake/warnings.cmake` - target-based warning level and warnings-as-errors policy
- `tools/CMakePresets.json` - shared local and CI-style presets
- `tools/docs/clean-reset.md` - documented reset workflow for stale build trees


---
## Repository layout
```text
.
├─ README.md
├─ chapters/
│  ├─ 00-intro-and-mental-model.md
│  ├─ 01-first-project.md
│  ├─ 02-build-systems-and-generators.md
│  ├─ 03-libraries.md
│  ├─ 04-usage-requirements.md
│  ├─ 05-configurations-debug-release.md
│  ├─ 06-testing-with-ctest.md
│  ├─ 07-googletest-and-fetchcontent.md
│  ├─ 08-external-scripts-and-generated-files.md
│  ├─ 09-versioning-in-cmake.md
│  ├─ 10-windows-exe-dll-version-info.md
│  ├─ 11-install-export-package.md
│  ├─ 12-SWIG-and-python-interface-generation.md
│  └─ 13-practical-commands-and-gotchas.md
├─ examples/
│  ├─ 01-first-project/
│  ├─ 02-build-systems-and-generators/
│  ├─ 03-libraries/
│  ├─ 04-usage-requirements/
│  ├─ 05-configurations-debug-release/
│  ├─ 06-testing-with-ctest/
│  ├─ 07-googletest-and-fetchcontent/
│  ├─ 08-external-scripts-and-generated-files/
│  ├─ 09-versioning-in-cmake/
│  ├─ 10-windows-exe-dll-version-info/
│  ├─ 11-install-export-package/
│  └─ 12-swig-and-python-interface-generation/
└─ cmake/
   └─ helpers/
````


---
## Base material and credits
This tutorial is inspired by and builds on the official CMake tutorial.
Primary base material:
* Official CMake Tutorial: [https://cmake.org/cmake/help/latest/guide/tutorial/index.html](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
* Official tutorial Step 1 ("Getting Started with CMake"): [https://cmake.org/cmake/help/latest/guide/tutorial/Getting%20Started%20with%20CMake.html](https://cmake.org/cmake/help/latest/guide/tutorial/Getting%20Started%20with%20CMake.html)
The official tutorial also exists in the CMake source tree.
The tutorial documentation and example source code live under:
* `Help/guide/tutorial`
Reference page describing that location:
* [https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/](https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/)
Credit goes to the CMake / Kitware documentation authors for the official tutorial structure and examples that serve as the conceptual starting point for this repository.
This repository does **not** attempt to replace the official documentation.
Instead, it uses the official tutorial as a base and expands it in a more practical, code-first direction for day-to-day development work.
---
## Required tools
You do not need every tool for every chapter, but this is the practical tool set used across the repository.
### Core required tools
* **CMake**
  Official download page: [https://cmake.org/download/](https://cmake.org/download/)
* **A C++ compiler and build environment**
  * **Visual Studio 2022** (Windows): [https://visualstudio.microsoft.com/downloads/](https://visualstudio.microsoft.com/downloads/)
  * or another compiler/toolchain supported by CMake on your platform
* **A generator / build backend**
  * **Ninja**: [https://ninja-build.org/](https://ninja-build.org/)
  * Ninja manual: [https://ninja-build.org/manual.html](https://ninja-build.org/manual.html)
### Recommended supporting tools
* **Git**
  Useful for cloning the repo and, later, for version-related examples: [https://git-scm.com/](https://git-scm.com/)
* **Python 3**
  Used in chapters that generate files or call external scripts: [https://www.python.org/downloads/](https://www.python.org/downloads/)
### Testing-related tools
* **GoogleTest**
  For the GoogleTest chapter we will show how to fetch it from the network using CMake, so a manual install is not required in advance: [https://github.com/google/googletest](https://github.com/google/googletest)
---
## Tool notes by chapter
* **Chapters 00-05**: CMake + compiler + generator are enough
* **Chapter 06**: same, plus CTest support from CMake
* **Chapter 07**: internet access is useful for fetching GoogleTest with `FetchContent`
* **Chapter 08**: Python is recommended for external script and generated-file examples
* **Chapter 10**: Windows resource compilation requires a Windows toolchain such as Visual Studio
---
## Prerequisites
You should have:
* CMake installed
* a C++ compiler
* at least one supported generator/build backend
Typical setups include:
### Windows
* Visual Studio 2022
* or Ninja + MSVC
* or Ninja + Clang
### Linux
* GCC or Clang
* Ninja or Make
### Helpful optional tools
* Git
* Python 3
* GoogleTest for local experimentation, though the tutorial will also show how to fetch it
---
## Core idea: CMake is not the compiler
CMake does not compile your code directly.
CMake:
* reads your project description
* generates build files for a chosen backend
* lets that backend do the actual build
Examples of generators/backends:
* Ninja
* Unix Makefiles
* Visual Studio
This is one of the most important ideas in the whole tutorial.
---
## Quick start
A typical workflow looks like this:
```bash
cmake -S . -B build
cmake --build build
```
This means:
* `-S .` = source directory is the current directory
* `-B build` = generate build files into the `build/` directory
If you use Ninja explicitly:
```bash
cmake -S . -B build -G Ninja
cmake --build build
```
If you use Visual Studio:
```bash
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug
```
Note that Visual Studio is a multi-config generator, so you usually specify `--config Debug` or `--config Release` when building.
---
## Recommended learning path
### Part 1 - Foundations
1. Introduction and mental model
2. First project
3. Build systems and generators
4. Libraries
5. Usage requirements
6. Debug and Release configurations
### Part 2 - Practical everyday workflows
7. Testing with CTest
8. GoogleTest and FetchContent
9. External scripts and generated files
10. Versioning in CMake
11. Windows EXE and DLL version info
### Part 3 - Shipping and scaling
12. Install, export, and package
13. Practical commands and gotchas
---
## Chapter index
### 00 - Introduction and mental model
What CMake is, what it is not, and the core ideas needed before writing any files.
### 01 - First project
A minimal executable and the smallest useful `CMakeLists.txt`.
### 02 - Build systems and generators
What Ninja, Visual Studio, and other generators are, and how generator choice affects the workflow.
### 03 - Libraries
Creating static and shared libraries and linking them into applications.
### 04 - Usage requirements
How `PUBLIC`, `PRIVATE`, and `INTERFACE` work, and why modern CMake is target-oriented.
### 05 - Configurations: Debug and Release
How configurations work, especially the difference between single-config and multi-config generators.
### 06 - Testing with CTest
The native test model in CMake and how to register and run tests.
### 07 - GoogleTest and FetchContent
How to bring in a testing framework from the network and integrate it cleanly.
### 08 - External scripts and generated files
How to call Python or other scripts, generate files, and connect them to targets.
### 09 - Versioning in CMake
How to define project version information and make it available to code and packaging.
### 10 - Windows EXE and DLL version info
How to stamp Windows binaries with version metadata using resource files.
### 11 - Install, export, and package
How to install artifacts, export targets, and prepare projects for reuse.
### 12 - SWIG Interface Generation
How to generate python interface as part of the build process.
### 13 - Practical commands and gotchas
A compact reference for useful commands, patterns, and common mistakes.
---
## A note on style
This tutorial prefers:
* small examples
* direct explanations
* target-based CMake
* out-of-source builds
* practical commands over long theory
Whenever possible, examples will show the modern target-oriented style rather than older global-variable-heavy approaches.
That said, older patterns may still be mentioned when useful for reading existing codebases.
---
## Common command cheat sheet
Configure:
```bash
cmake -S . -B build
```
Configure with a generator:
```bash
cmake -S . -B build -G Ninja
```
Build:
```bash
cmake --build build
```
Build a specific configuration:
```bash
cmake --build build --config Release
```
Install:
```bash
cmake --install build
```
Run tests:
```bash
ctest --test-dir build
```
Pass a cache variable:
```bash
cmake -S . -B build -DMY_OPTION=ON
```
Clean by removing the build tree:
* delete the `build/` directory
---
## What this tutorial does not try to do
This repo does not try to:
* document every single CMake command
* replace the official CMake documentation
* cover every platform-specific edge case
* teach C++ itself
Instead, it tries to give you a clean, practical path through the parts of CMake that are most useful in day-to-day engineering work.
---
## References
Official CMake material used as the base for this tutorial:
* CMake Tutorial: [https://cmake.org/cmake/help/latest/guide/tutorial/index.html](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
* Step 1: Getting Started with CMake: [https://cmake.org/cmake/help/latest/guide/tutorial/Getting%20Started%20with%20CMake.html](https://cmake.org/cmake/help/latest/guide/tutorial/Getting%20Started%20with%20CMake.html)
* Tutorial source location inside the CMake tree: [https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/](https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/)
Official tool pages used throughout the repo:
* CMake download: [https://cmake.org/download/](https://cmake.org/download/)
* Ninja: [https://ninja-build.org/](https://ninja-build.org/)
* Ninja manual: [https://ninja-build.org/manual.html](https://ninja-build.org/manual.html)
* Visual Studio downloads: [https://visualstudio.microsoft.com/downloads/](https://visualstudio.microsoft.com/downloads/)
* Python downloads: [https://www.python.org/downloads/](https://www.python.org/downloads/)
* GoogleTest: [https://github.com/google/googletest](https://github.com/google/googletest)
* Git: [https://git-scm.com/](https://git-scm.com/)
---
## Status
This repository is being built chapter by chapter.
Planned initial focus:
* foundations
* generators
* libraries
* tests
* fetched dependencies
* external scripts
* versioning
* Windows binary metadata
---
## Suggested way to use this repo in a team
A good team workflow is:
* read one chapter
* run the example
* discuss what changed from the previous example
* adapt the pattern into a real project
* keep notes on local conventions and preferred defaults
This makes the tutorial useful both for onboarding and for cleaning up existing build systems.
