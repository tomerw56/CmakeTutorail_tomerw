# Chapter 12 - SWIG and Python Interface Generation
In the previous chapter we made a project installable, exportable, and consumable from another CMake project.
Now we move to another very practical team workflow:
**generating Python bindings from C or C++ code with SWIG**.
This chapter is about a common situation in mixed-language teams:
- core logic lives in C or C++
- Python is used for scripting, testing, orchestration, or data work
- the team wants a generated interface instead of hand-writing bindings
CMake has dedicated support for SWIG through the `FindSWIG` and `UseSWIG` modules, and Python integration is typically paired with `FindPython3`. 
---
## Exampels Note
**Please see the examples ReadMe.md file as it contains swig setup notes.**
## Why this matters
A lot of real projects need a bridge between native code and Python.
Typical reasons include:
- exposing performance-sensitive C++ code to Python
- building test harnesses in Python around native code
- making internal libraries scriptable
- integrating native components into data-science workflows
- keeping one implementation while supporting multiple front ends
SWIG is one of the classic tools for this job, and CMake’s SWIG support is designed specifically to help generate wrapper libraries as part of the build. `swig_add_library()` creates a target with the same capabilities as a normal `add_library()` target, so it can participate in target-based CMake just like other libraries. :contentReference[oaicite:1]{index=1}
---
## Before you run the sample

This chapter depends on external tools and environment setup:
- SWIG
- Python 3
- a working native toolchain

Before building the sample, read the example-specific setup notes here:

- `examples/12-swig-and-python-interface-generation/README.md`

That README covers:
- installing SWIG
- pointing CMake at `swig.exe`
- Python interpreter selection
- why the Python version/interpreter used at runtime must match the one CMake found during configure
- the recommended Windows workflow for this chapter

## What we will cover
In this chapter we will cover:
- `find_package(SWIG ...)`
- `include(UseSWIG)`
- `find_package(Python3 ...)`
- a small SWIG interface file
- generating a Python module from C++ code
- running a short Python script that imports the generated module
This chapter is intentionally practical:
one small native library, one `.i` interface file, one generated Python module, one small Python consumer. `FindSWIG` supports selecting required language components such as `python`, and `FindPython3` can locate both the interpreter and development artifacts needed for module builds. :contentReference[oaicite:2]{index=2}
---
## The key idea
The core pattern is:
1. write or reuse a native C/C++ library
2. describe the interface you want SWIG to expose
3. let SWIG generate wrapper code
4. compile that wrapper into a Python extension module
5. import and use that module from Python
CMake’s `swig_add_library()` command is the center of that workflow. It defines a SWIG module target, supports specifying the target language, and the resulting target can be used with commands such as `target_link_libraries()`. :contentReference[oaicite:3]{index=3}
---
## Why SWIG belongs in its own chapter
This is not just a small variation of the generated-files chapter.
It connects several ideas we already built:
- generated files
- external tooling
- target-based linking
- installed/consumable outputs
- Python integration
It also introduces a distinct kind of build product:
a Python-loadable native module, which is not quite the same as a normal executable or shared library. `FindPython3` exposes imported targets such as `Python3::Interpreter` and `Python3::Module`, which makes this fit naturally into modern target-based CMake. :contentReference[oaicite:4]{index=4}
---
## The first CMake pieces
A modern SWIG-enabled CMake project usually starts with:
- `find_package(SWIG COMPONENTS python)`
- `include(UseSWIG)`
- `find_package(Python3 COMPONENTS Interpreter Development.Module)`
`FindSWIG` supports optional language components and documents `python` as one of the supported lowercase language names for `swig_add_library()`. `FindPython3` documents `Interpreter` and `Development.Module` as the relevant components for Python module development, and provides the imported targets `Python3::Interpreter` and `Python3::Module`. :contentReference[oaicite:5]{index=5}
---
## Why `Development.Module` matters
For a Python extension module, the Python interpreter alone is not enough.
You also need the development artifacts used for Python module builds.
That is exactly what `FindPython3`’s `Development.Module` component is for, and the module target `Python3::Module` is defined when that component is found. :contentReference[oaicite:6]{index=6}
That is why a good chapter example should not stop at:
```cmake
find_package(Python3 COMPONENTS Interpreter)
````
It should request the module-development side too.
---
## What `UseSWIG` adds
`FindSWIG` locates the SWIG tool.
`UseSWIG` adds the commands and behavior used to integrate SWIG into the build.
The main command is:
```cmake
swig_add_library(...)
```
`UseSWIG` documents `swig_add_library(<name> ... LANGUAGE <language> ... SOURCES <file>...)` as the command used to define a SWIG module, and it notes that the resulting target behaves like a normal library target for later commands. ([CMake][1])
That is one of the nicest parts of this workflow:
even though SWIG is generating code, the final integration still fits the normal CMake target model.
---
## A likely chapter example
A good Chapter 12 example should probably contain:
* one small C++ math library
* one SWIG interface file, such as `math.i`
* one generated Python module target
* one Python script that imports the module and calls the wrapped functions
That keeps the example small and gives a very clear success condition:
> Python imports the generated module and uses native C++ code through it.
---
## A likely folder layout
A clean layout could be:
```text
examples/
└─ 12-swig-and-python-interface-generation/
   ├─ CMakeLists.txt
   ├─ ReadMe.md
   ├─ cpp/
   │  ├─ math_utils.cpp
   │  └─ math_utils.h
   ├─ swig/
   │  └─ math.i
   └─ python/
      └─ demo.py
```
This keeps the roles clear:
* `cpp/` for native source
* `swig/` for the wrapper interface
* `python/` for the Python-side demo
---
## The SWIG interface file
The `.i` file is where SWIG learns what to wrap.
It usually contains:
* module declaration
* included headers
* interface directives
* declarations or inclusions describing the native API
This file is the bridge between:
* native code
* generated wrapper code
* the final Python module
In CMake terms, files with extension `.i` are identified as SWIG sources by `swig_add_library()`, unless that behavior is explicitly overridden. ([CMake][1])
---
## C++ mode for SWIG
If the wrapped code is C++, the SWIG source file should be treated as C++ mode.
`UseSWIG` documents the `CPLUSPLUS` source-file property for this exact purpose, for example:
```cmake
set_property(SOURCE mymod.i PROPERTY CPLUSPLUS ON)
```
That is an important detail for the chapter because it makes the example clearly about **C++ to Python**, not just C to Python. ([CMake][1])
---
## Target include directories and SWIG
A subtle but important point is that SWIG may need access to the same include directories as the native target.
`UseSWIG` documents the `USE_TARGET_INCLUDE_DIRECTORIES` source property and the `SWIG_USE_TARGET_INCLUDE_DIRECTORIES` target property. If enabled, the target’s include directories are forwarded to the SWIG compiler. ([CMake][1])
That means a good tutorial should make this explicit instead of hoping include paths magically line up.
---
## Output directories matter
`UseSWIG` provides `OUTPUT_DIR` and `OUTFILE_DIR` controls, and it also documents `UseSWIG_MODULE_VERSION`.
A very important practical note from the docs is:
* if `UseSWIG_MODULE_VERSION` is `2`, support files are placed in a dedicated directory
* the output directory contents may be erased as part of the target build
* therefore, each SWIG target should use its own dedicated output directory
That is exactly the kind of practical gotcha a team-facing tutorial should call out early. ([CMake][1])
So for this chapter, I would strongly prefer one dedicated build/output directory per SWIG target.
---
## Multi-config generator caution
There is also a very useful limitation documented by `UseSWIG`:
For multi-config generators, SWIG does not support configuration-specific generated source files; all build configurations must produce the same generated source file. ([CMake][1])
That means this chapter should probably present the cleanest path first as:
* default generator
* or Ninja
and only mention the multi-config caveat as a note, rather than building the entire lesson around Visual Studio-specific variation.
---
## A likely CMake shape
A practical Chapter 12 `CMakeLists.txt` will probably do something like this:
1. find SWIG
2. include `UseSWIG`
3. find Python3 interpreter and module-development artifacts
4. build or define a small native library
5. set SWIG properties on the `.i` file
6. call `swig_add_library(... LANGUAGE python ...)`
7. link the generated module against the native library and Python module target
8. run a Python demo script
That is the end-to-end flow we want.
---
## Why this chapter fits after install/export/package
This chapter belongs near the end because it builds on several earlier ideas:
* chapter 08: external tooling and generated files
* chapter 09: configure/build data flow
* chapter 11: consumable outputs and packaging mindset
By the time we reach SWIG, the reader already understands:
* targets
* generation steps
* external tools
* consumer workflows
So the chapter can stay focused on the interface-generation problem itself.
---
## Good habits in this chapter
### 1. Keep the native API simple at first
The first SWIG demo should wrap a small, predictable interface.
### 2. Use explicit Python components
Request `Interpreter` and `Development.Module` together when building a Python extension module. `FindPython3` explicitly documents both and recommends asking for related components together to ensure consistent versions. ([CMake][2])
### 3. Use dedicated output directories
Especially when using `UseSWIG_MODULE_VERSION=2`, because support-file output directories may be erased during builds. ([CMake][1])
### 4. Make SWIG’s include-path behavior explicit
Do not rely on accidental include discovery.
### 5. Keep the first Python-side demo tiny
The goal is to prove the module works, not to build a whole Python app.
---
## Common mistakes
### 1. Finding SWIG but forgetting `include(UseSWIG)`
`FindSWIG` locates the tool; `UseSWIG` provides `swig_add_library()`. ([CMake][3])
### 2. Requesting only the Python interpreter
A Python extension module usually also needs Python development/module artifacts, which `FindPython3` provides through `Development.Module`. ([CMake][2])
### 3. Forgetting C++ mode on the `.i` file
That can break or distort wrapping of C++ headers. `UseSWIG` documents the `CPLUSPLUS` source property for this. ([CMake][1])
### 4. Reusing SWIG output directories carelessly
That can cause target interference or lost generated files, especially with `UseSWIG_MODULE_VERSION=2`. ([CMake][1])
### 5. Assuming SWIG-generated sources can vary freely by configuration
`UseSWIG` explicitly warns that multi-config generators do not support configuration-specific generated source files. ([CMake][1])
---
## What this chapter intentionally keeps simple
This first SWIG chapter does **not** need to cover everything.
We are intentionally keeping out:
* advanced typemaps
* large class hierarchies
* director features
* packaging Python wheels
* multiple language targets in one example
* complicated ownership/lifetime cases
* advanced Python ABI strategies
Those are all real topics, but the first clean lesson is:
* wrap a small C++ API
* generate a Python module
* import it successfully from Python
---
## This is enough for now
At this stage, the key ideas to keep are:
* CMake has dedicated SWIG support through `FindSWIG` and `UseSWIG`
* Python module builds should use `FindPython3` with the right components
* `swig_add_library()` is the center of the CMake-side SWIG workflow
* SWIG `.i` files are the interface description that drives wrapper generation
* output directories and include propagation matter
* a small end-to-end example is the right first teaching path
That is the mental model we want before writing the code. ([CMake][3])
---
## Takeaways
This chapter is where the tutorial moves from:
* “build native code”
to:
* “generate a language binding around native code”
That is a major step, and a very real one for teams that use Python around C or C++ libraries.
It also shows that CMake is not just a compiler driver:
it can coordinate a wrapper generator, native compilation, and a Python-facing module in one workflow.
---
## Next step in this chapter
The code for this chapter should define:
* a small C++ native library
* a SWIG interface file
* CMake logic using `FindSWIG`, `UseSWIG`, and `FindPython3`
* a generated Python module target
* a short Python script that imports the module and calls into it

[1]: https://cmake.org/cmake/help/latest/module/UseSWIG.html "UseSWIG — CMake 4.3.2 Documentation"
[2]: https://cmake.org/cmake/help/latest/module/FindPython3.html "FindPython3 — CMake 4.3.2 Documentation"
[3]: https://cmake.org/cmake/help/latest/module/FindSWIG.html "FindSWIG — CMake 4.3.2 Documentation"
