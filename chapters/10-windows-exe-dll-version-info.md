# Chapter 10 - Windows EXE and DLL Version Info
In the previous chapter we defined version information in CMake and made it available to code through a generated header.
Now we build on that foundation and move to a Windows-specific topic:
**stamping version information into executables and DLLs**.
This chapter is about the metadata you see in Windows file properties:
- file version
- product version
- product name
- company name
- description
The key idea is simple:
- Chapter 09 gave us a version defined in CMake
- Chapter 10 uses that version to generate a Windows resource file and compile it into an EXE or DLL
That gives us one source of truth for both runtime version output and Windows binary metadata. CMake’s `project()` command accepts a numeric `VERSION` and exposes `PROJECT_VERSION` plus the `MAJOR`, `MINOR`, `PATCH`, and `TWEAK` components, which makes it a natural source for this metadata. :contentReference[oaicite:0]{index=0}
 
 ---
## Why this matters
For Windows projects, version info is not just cosmetic.
It helps with:
- identifying builds in Explorer
- checking which binary was deployed
- matching an EXE or DLL to a release
- making support and troubleshooting easier
- keeping binary metadata aligned with the project version
If that metadata is filled in by hand, it drifts.
If it is generated from the same CMake version data used elsewhere, it stays much more consistent. `configure_file()` is specifically designed to copy a template to the build tree while substituting variables, and CMake will re-run configuration when the input template changes. :contentReference[oaicite:1]{index=1}
 
 ---
## What we will cover
In this chapter we will cover:
- what a Windows resource file is
- how to generate a `.rc` file from CMake values
- how to add that `.rc` file to an executable
- how to add that `.rc` file to a DLL
- how to keep the binary metadata aligned with `project(VERSION ...)`
We will keep the example focused:
one EXE, one DLL, and version data generated from the same CMake version source. CMake allows executables and libraries to be built from listed source files, and source lists can also be supplied later with `target_sources()` if desired. :contentReference[oaicite:2]{index=2}
 
 ---
## The key idea
The pattern for this chapter is:
1. define the project version in CMake
2. create a `.rc.in` template
3. use `configure_file()` to produce a final `.rc` file in the build tree
4. add that `.rc` file as a source of the EXE or DLL target
5. build and inspect the file properties in Windows
This is a very natural follow-up to Chapter 09 because the version already exists in CMake variables such as `PROJECT_VERSION`, `PROJECT_VERSION_MAJOR`, `PROJECT_VERSION_MINOR`, and `PROJECT_VERSION_PATCH`. :contentReference[oaicite:3]{index=3}
 
 ---
## What a Windows resource file is
A Windows resource file, usually with the extension `.rc`, is a file compiled by the Windows resource compiler and linked into the final binary.
For this chapter, the important use of the resource file is:
**version metadata**.
That means the binary can carry structured Windows-visible information such as:
- `FILEVERSION`
- `PRODUCTVERSION`
- `FileDescription`
- `ProductName`
- `FileVersion`
- `ProductVersion`
We do not need to teach every Windows resource feature here.
We only need the version-info path. The practical CMake side is simply that the generated `.rc` file is treated as part of the target’s sources. CMake documents executables and libraries as being built from the listed source files, with target outputs placed in the build tree by default. :contentReference[oaicite:4]{index=4}
 
 ---
## Why this chapter belongs after Chapter 09
Chapter 09 taught:
- define version once
- expose version through CMake variables
- generate a version header for code
Chapter 10 takes the same version values and applies them to **Windows binary metadata**.
That progression is important because it keeps one source of truth.
We are not introducing a second disconnected version system for Windows.
We are extending the same one. CMake’s `project()` command is exactly what exposes those version components in a reusable form. :contentReference[oaicite:5]{index=5}
 
 ---
## The right tool: `configure_file()`
For this chapter, `configure_file()` is the right tool again.
Why?
Because the values we want to place into the `.rc` file already exist during the configure step:
- project version
- product name
- company name
- file description
This is a configure-time transformation, not a build-time generator problem.
That makes it a better fit for `configure_file()` than for a custom build command. `configure_file()` copies the input to the output and performs substitutions; it also causes CMake to re-run if the input file changes. :contentReference[oaicite:6]{index=6}
 
 ---
## Folder layout
A clean example layout could be:
 
 ``` text
examples/
└─ 10-windows-exe-dll-version-info/
   ├─ CMakeLists.txt
   ├─ app/
   │  └─ main.cpp
   ├─ dll/
   │  ├─ demo_api.cpp
   │  └─ demo_api.h
   └─ resources/
      ├─ app_version.rc.in
      └─ dll_version.rc.in
 ```
This keeps the chapter readable:
* application code
* DLL code
* resource templates
The generated `.rc` files themselves should go into the build tree, not the source tree, just like the configured version header in Chapter 09. `configure_file()` writes outputs relative to `CMAKE_CURRENT_BINARY_DIR` when given relative output paths, and it will create missing output directories as needed. ([CMake][1])
 
 ---
## EXE and DLL targets
The example will likely contain:
* one executable target
* one shared library target
The `.rc` file for each target will be configured separately and then added to that target’s source list.
That means the flow is still target-oriented:
* define target
* generate target-specific resource file
* compile the target with that resource file included
CMake’s `add_executable()` and `add_library()` commands both build targets from the listed sources, and both allow the source list to be supplied directly or later via `target_sources()`. ([CMake][2])
 
 ---
## A note about DLLs on Windows
There is one Windows-specific caveat that is worth knowing.
For normal DLLs that export symbols, `SHARED` is the natural choice.
But CMake documents that if a library exports no symbols, it should not be declared as `SHARED` on Windows; for example, a resource-only Windows DLL should be a `MODULE` library instead, because CMake expects a `SHARED` library on Windows to have an associated import library. ([CMake][3])
For this tutorial, that means:
* a real DLL with exported functions → `SHARED`
* a resource-only DLL → `MODULE`
We will keep the chapter in the first category so the example stays practical. ([CMake][3])
 
 ---
## What the resource template will do
The `.rc.in` template will contain placeholders such as:
* `@PROJECT_VERSION@`
* `@PROJECT_VERSION_MAJOR@`
* `@PROJECT_VERSION_MINOR@`
* `@PROJECT_VERSION_PATCH@`
and perhaps a few product-specific strings like:
* application name
* file description
* product name
Then `configure_file()` will write the final `.rc` file into the build tree using the current CMake values. This is the same template-substitution model described in the `configure_file()` documentation. ([CMake][1])
 
 ---
## Why we usually separate EXE and DLL resource templates
We *could* use one generic resource template, but separate templates are often clearer for teaching.
Why?
Because the strings can differ:
* EXE description might describe the application
* DLL description might describe the library/module
* product name may stay the same
* file description may differ per binary
This makes the metadata more realistic while still keeping the version data shared. The version component variables all come from the same `project(VERSION ...)` call. ([CMake][4])
 
 ---
## What the chapter demo should show
A good Chapter 10 demo should let the reader do two things:
 
 ### 1. Run the binaries
So the example still feels like a normal tutorial project.
 
 ### 2. Inspect the file properties in Windows
So the version metadata becomes visible outside the code.
That second step is the real payoff of the chapter:
the build’s version source of truth is now reflected in Windows-visible binary metadata as well as runtime code output. The target binaries are created in the build tree by default unless output directory properties are changed. ([CMake][2])
 
 ---
## Good habits in this chapter
 
 ### 1. Keep version defined once
The version should still start from `project(VERSION ...)`.
 
 ### 2. Generate `.rc` files into the build tree
Do not hand-edit configured resource files under source control.
 
 ### 3. Keep product strings explicit
Names like product name and file description should be readable and intentional.
 
 ### 4. Keep target ownership clear
Each target should get the resource file intended for it.
 
 ### 5. Reuse the same version data in code and metadata
That is the whole point of this chapter.
These habits align well with the configure-time generation model described by `configure_file()`. ([CMake][1])
 
 ---
## Common mistakes
 
 ### 1. Hardcoding the version separately in the `.rc` file
That defeats the single-source-of-truth goal.
 
 ### 2. Generating the resource file into the source tree
That mixes generated outputs with source files.
 
 ### 3. Using a build-time generator for purely configure-time metadata
That adds unnecessary complexity.
 
 ### 4. Forgetting the DLL caveat on Windows
A library with no exported symbols should not be modeled as a normal `SHARED` library on Windows. ([CMake][3])
 
 ### 5. Forgetting that output paths depend on the generator
Especially with Visual Studio, binaries may appear under `Debug/` or `Release/`. CMake documents that executables and libraries are created in the corresponding build-tree directories by default, with output directory properties available to change that. ([CMake][2])
 
 ---
## What this chapter intentionally keeps simple
This first Windows version-info chapter does **not** need to cover everything.
We are intentionally keeping out:
* icons and dialogs in resource files
* localization details
* advanced installer metadata
* manifests
* signing
* auto-generated Git revision strings
* package export metadata
Those are all real topics, but the first clean lesson is:
* generate a `.rc` file from CMake version values
* compile it into an EXE or DLL
* verify the metadata in Windows
 
 ---
## This is enough for now
At this stage, the key ideas to keep are:
* `project(VERSION ...)` gives us reusable version variables
* `configure_file()` is the right tool for turning those values into a Windows resource file
* the configured `.rc` file should live in the build tree
* EXEs and DLLs can include those `.rc` files as target sources
* the result is version metadata visible in Windows file properties
* the same CMake version source can drive both code and binary metadata
That is the mental model we want before writing the code. ([CMake][4])
 
 ---
## Takeaways
This chapter is where versioning becomes visible outside the program itself.
We are moving from:
* “the code can print the version”
to:
* “the Windows binary carries the version too”
That makes the build feel more complete and much closer to what real Windows projects do.
 
 ---
## Next step in this chapter
The code for this chapter defines:
* a versioned CMake project
* one EXE target
* one DLL target
* one `.rc.in` template for each target
* `configure_file()` calls to generate the real `.rc` files
* build commands for Ninja and Visual Studio
* a Windows verification step: inspect file properties

[1]: https://cmake.org/cmake/help/latest/command/configure_file.html "configure_file — CMake 4.3.2 Documentation"
[2]: https://cmake.org/cmake/help/latest/command/add_executable.html "add_executable — CMake 4.3.2 Documentation"
[3]: https://cmake.org/cmake/help/latest/command/add_library.html "add_library — CMake 4.3.2 Documentation"
[4]: https://cmake.org/cmake/help/latest/command/project.html "project — CMake 4.3.2 Documentation"
