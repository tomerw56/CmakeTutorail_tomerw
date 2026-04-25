# Chapter 09 - Versioning in CMake
In the previous chapter we generated files with an external script and connected the generated output back into the build.
Now we move to another very practical topic:
**versioning**.
This chapter is about how a CMake project can define version information once and then reuse it in useful places such as:
- generated headers
- runtime output
- packaging
- install/export metadata later on
CMake’s `project()` command accepts a `VERSION` argument made of numeric components like `major.minor.patch[.tweak]`, and it sets project version variables such as `PROJECT_VERSION`, `PROJECT_VERSION_MAJOR`, `PROJECT_VERSION_MINOR`, `PROJECT_VERSION_PATCH`, and `PROJECT_VERSION_TWEAK`.
 
 
 ---
## Why this matters
A project version is more than a string in a README.
In real projects, version information often needs to appear in several places:
- code
- logs
- binary metadata
- package names
- install/export files
If version data is copied manually into multiple places, it drifts.
If it is defined once and reused, the project is easier to maintain.
That is the core goal of this chapter.
 
 
 ---
## What we will cover
In this chapter we will cover:
- defining a project version in `project()`
- the variables CMake exposes from that version
- generating a version header with `configure_file()`
- including that header from application code
- printing version information at runtime
`project(VERSION ...)` sets the project version variables, and `configure_file()` is the standard configure-time tool for transforming an input template into an output file in the build tree. If the input changes, the build system will re-run CMake to regenerate it. :contentReference[oaicite:1]{index=1}
 
 
 ---
## The key idea
The best simple pattern is:
1. define the version in one place
2. let CMake expose version variables
3. generate a header from those variables
4. use that generated header in code
That gives us one source of truth.
 
 
 ---
## Defining the version in `project()`
A typical versioned project starts like this:

 ``` cmake
project(MyApp VERSION 1.2.3 LANGUAGES CXX)

 ``` `
CMake documents that `project()` stores the name and also sets version variables when `VERSION` is given. It also supports a fourth numeric component, often referred to as `tweak`. ([CMake][1])
That means we can use values such as:
* `PROJECT_VERSION`
* `PROJECT_VERSION_MAJOR`
* `PROJECT_VERSION_MINOR`
* `PROJECT_VERSION_PATCH`
* `PROJECT_VERSION_TWEAK`
These variables come from the most recent `project()` call, while `CMAKE_PROJECT_VERSION` and its component variables refer to the top-level project. ([CMake][2])
 
 
 ---
## `PROJECT_VERSION` vs `CMAKE_PROJECT_VERSION`
For many small examples, `PROJECT_VERSION` is enough.
But it is useful to know the distinction:
* `PROJECT_VERSION` refers to the most recent `project()` call
* `CMAKE_PROJECT_VERSION` refers to the top-level project
That difference matters more once a project grows and contains multiple nested `project()` calls. CMake documents both sets of variables separately. ([CMake][2])
For this tutorial’s small examples, `PROJECT_VERSION` is the natural first choice.
 
 
 ---
## Why a generated header is a good first versioning demo
We could print the version from CMake messages during configure, but that would not help the program itself know its own version.
A generated header is better because it:
* moves version data into compiled code
* keeps the version tied to the build
* demonstrates a useful configure-time pattern
This is a very natural use of `configure_file()`, which copies a template to an output file while transforming the contents. ([CMake][3])
 
 
 ---
## The pattern we will use
A clean Chapter 09 example will likely have:
* a top-level `project(... VERSION ...)`
* a template file such as `generated_version.h.in`
* a `configure_file()` call that writes `generated_version.h` into the build tree
* an executable that includes the generated header
* runtime output that prints the version
This is small, visible, and practical.
 
 
 ---
## Why `configure_file()` is the right tool here
This is a **configure-time** transformation.
That makes `configure_file()` a better fit than a build-time generator.
The version values already exist inside CMake during configure, so a configure-time template expansion is exactly the right model. CMake’s docs describe `configure_file()` as copying an input file to an output file while performing transformations, and they note that if the input file changes, CMake will re-run to regenerate it. ([CMake][3])
That also means this chapter pairs nicely with Chapter 08:
* Chapter 08 emphasized build-time generation with custom commands
* Chapter 09 emphasizes configure-time generation from project metadata
 
 
 ---
## A likely folder layout
A simple example layout could be:

 ``` text
examples/
└─ 09-versioning-in-cmake/
   ├─ CMakeLists.txt
   ├─ app/
   │  └─ main.cpp
   └─ generated/
      └─ generated_version.h.in

 ``` 
This keeps the example focused and easy to compare against Chapter 08.
 
 
 ---
## What the template might contain
A configured header template often contains placeholders such as:

 ``` cpp
#pragma once
#define APP_VERSION "@PROJECT_VERSION@"
#define APP_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define APP_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define APP_VERSION_PATCH @PROJECT_VERSION_PATCH@

 ``` 
Then `configure_file()` writes the final header into the build tree using the values from the current configuration step. That is the normal configure-file workflow described by CMake. ([CMake][3])
 
 
 ---
## What the program will do
The executable can stay small.
It will:
* include the generated version header
* print the full version
* optionally print the major/minor/patch pieces separately
That gives immediate proof that:
* the version was defined in CMake
* the header was generated
* the code compiled against the generated file
* the executable is using build-defined version information
 
 
 ---
## Good habits in this chapter
 
 
 ### 1. Keep the version defined in one place
The `project(VERSION ...)` line should be the starting point.
 
 
 ### 2. Generate version data into the build tree
Do not hand-edit generated version headers in the source tree.
 
 
 ### 3. Use configure-time generation for configure-time data
This is exactly what `configure_file()` is good at. ([CMake][3])
 
 
 ### 4. Keep the first version example simple
A small generated header is enough to teach the pattern.
 
 
 ### 5. Reuse the same version data later
This chapter is the foundation for later packaging and Windows binary metadata topics.
 
 
 ---
## Common mistakes
 
 
 ### 1. Duplicating the version in multiple files by hand
That leads to drift.
 
 
 ### 2. Using a build-time generator for configure-time project metadata
That is usually unnecessary complexity.
 
 
 ### 3. Writing generated headers into the source tree
That mixes source and generated artifacts.
 
 
 ### 4. Forgetting to add the generated header location to the include path
The compiler still needs to know where the configured file lives.
 
 
 ### 5. Confusing project-local version variables with top-level project version variables
`PROJECT_VERSION` and `CMAKE_PROJECT_VERSION` are related, but they are not identical in all layouts. ([CMake][2])
 
 
 ---
## Configure-time vs build-time versioning
This is a good place to reinforce a distinction:
 
 
 ### Configure-time versioning
Good for:
* `project(VERSION ...)`
* configured headers
* package metadata templates
* install/export metadata later
 
 
 ### Build-time versioning
Good for:
* generated revision stamps from a tool
* timestamps
* outputs that depend on a script at build time
This chapter is mostly about the **configure-time** case.
 
 
 ---
## Why this chapter belongs before Windows binary version info
Chapter 09 is the clean foundation.
It teaches:
* version defined in CMake
* version exposed to code
* one-source-of-truth thinking
Then a later Windows-focused chapter can build on that to stamp version information into EXE and DLL resources.
That progression is much cleaner than jumping directly into `.rc` files.
 
 
 ---
## What we intentionally keep simple
This chapter does **not** need to cover all versioning concerns at once.
We are intentionally keeping out:
* Git-derived versions
* auto-incrementing build numbers
* semantic-version policy debates
* package config helper details
* Windows resource stamping
* installer version metadata
* runtime revision probes
Those are all valid topics, but the first clean win is:
* define version
* configure header
* print it from code
 
 
 ---
## This is enough for now
At this stage, the key ideas to keep are:
* `project(VERSION ...)` defines project version information
* CMake exposes that version through variables such as `PROJECT_VERSION` and its components
* `configure_file()` is the natural tool for turning those values into a generated header
* generated version headers belong in the build tree
* one source of truth makes versioning much easier to maintain
That is the mental model we want before writing the code. ([CMake][1])
 
 
 ---
## Takeaways
This chapter is about turning project metadata into something the program can actually use.
We are moving from:
* “the project has a version somewhere”
to:
* “the build defines the version”
* “the code sees the version”
* “the executable can report the version”
That is a strong and very practical versioning foundation.
 
 
 ---
## Next step in this chapter
The code for this chapter should define:
* a `project(... VERSION ...)`
* a configured header template
* a `configure_file()` call
* an executable that includes the generated version header
* runtime output showing the version and its components

[1]: https://cmake.org/cmake/help/latest/command/project.html?utm_source=chatgpt.com "project — CMake 4.3.2 Documentation"
[2]: https://cmake.org/cmake/help/latest/variable/PROJECT_VERSION.html?utm_source=chatgpt.com "PROJECT_VERSION — CMake 4.3.2 Documentation"
[3]: https://cmake.org/cmake/help/latest/command/configure_file.html?utm_source=chatgpt.com "configure_file — CMake 4.3.2 Documentation"
