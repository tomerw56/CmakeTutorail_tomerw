# Chapter 11 - Install, Export, and Package
In the previous chapter we used CMake version information to stamp Windows executables and DLLs.
Now we move to the next natural question:
> how do we make the results of our build usable outside the build tree?
This chapter is about the path from:
- “I built a target”
to:
- “I can install it”
- “another project can consume it”
- “I can package it for distribution”
This is where a project starts to behave like something other people can actually use.
---
## Why this matters
Up to this point, most of our examples have lived inside the build tree.
That is useful for learning, but real projects eventually need to:
- install executables and libraries to predictable locations
- install headers for consumers
- export targets so that other CMake projects can import them
- provide package configuration files
- create distributable packages
A clean install/export/package story turns a project from “something that builds here” into “something that can be consumed elsewhere”.
---
## What we will cover
In this chapter we will cover:
- `install(TARGETS ...)`
- `install(FILES ...)`
- `install(EXPORT ...)`
- package config files
- package version files
- a first look at CPack
We will keep the first pass practical and focused.
---
## The key mental model
A useful way to think about this chapter is:
### Build tree
Where the project is compiled.
### Install tree
Where the project is laid out for use.
### Package
A distributable artifact created from the install information.
This distinction is important because CMake treats build-tree exports and install-tree exports differently.
---
## Install tree vs build tree
CMake supports both of these use cases:
### Build-tree export
Useful for directly reusing targets from the current build tree.
### Install-tree export
Useful for installed artifacts that another project can consume after installation.
That distinction matters because `export(TARGETS ...)` is for the **build tree**, while `install(EXPORT ...)` is for the **install tree**. The `export()` documentation says it exports targets “from the current project's build tree” and explicitly says the file it creates is build-tree-specific and “should never be installed.” :contentReference[oaicite:0]{index=0}
---
## Why installation comes first
Before a project can be packaged or cleanly reused, it needs install rules.
Install rules define:
- what gets installed
- where it goes
- which parts belong to runtime vs development use
- which files should be visible to consumers
CMake’s `install()` command is the mechanism that generates installation rules for a project. The install rules declared through `install()` are then executed during installation. :contentReference[oaicite:1]{index=1}
---
## `install(TARGETS ...)`
This is the core command for installing built targets.
Typical targets include:
- executables
- shared libraries
- static libraries
CMake’s `install(TARGETS ...)` signature is specifically for installing target output artifacts and associated files. The documentation also distinguishes artifact kinds such as `RUNTIME`, `LIBRARY`, and `ARCHIVE`. :contentReference[oaicite:2]{index=2}
A practical first pattern looks like this:
```cmake
install(TARGETS my_app my_lib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
````
This is the first install command most readers need to get comfortable with.
---
## `RUNTIME`, `LIBRARY`, and `ARCHIVE`
These keywords matter because different target artifacts land in different places.
A simple working mental model is:
* `RUNTIME` → executables and DLL runtime artifacts
* `LIBRARY` → shared libraries on non-DLL platforms
* `ARCHIVE` → static libraries and import libraries
CMake’s install documentation explicitly documents these artifact kinds and shows that on DLL platforms, shared libraries can involve both `RUNTIME` and `ARCHIVE` pieces. It also documents built-in default destinations such as `${CMAKE_INSTALL_BINDIR}` → `bin` for `RUNTIME`. ([CMake][1])
This is one of the reasons install rules can look more detailed than build rules:
they are describing the install layout, not just compilation.
---
## Relative install destinations
A very important best practice is to use **relative** install destinations such as:
* `bin`
* `lib`
* `include`
not absolute hardcoded paths.
The install documentation strongly recommends relative `DESTINATION` paths because they work better with `cmake --install --prefix`, package maintainers, and CPack. Relative destinations are interpreted relative to `CMAKE_INSTALL_PREFIX`. ([CMake][1])
So this is good:
```cmake
install(TARGETS my_app RUNTIME DESTINATION bin)
```
and this is usually a poor tutorial default:
```cmake
install(TARGETS my_app RUNTIME DESTINATION "C:/some/fixed/path")
```
---
## `CMAKE_INSTALL_PREFIX`
When install destinations are relative, they are interpreted relative to `CMAKE_INSTALL_PREFIX`.
That means a project can be installed under different prefixes without rewriting every install rule. The `install()` documentation explicitly states that relative destinations are interpreted relative to `CMAKE_INSTALL_PREFIX`. ([CMake][1])
This is one reason install rules scale well:
the rules describe the layout, while the prefix controls the root location.
---
## Installing headers and other files
Targets are only part of the install story.
A library that is meant to be consumed by others often also needs:
* public headers
* config files
* package version files
That is where commands like `install(FILES ...)` come in.
So a normal install story often includes both:
* `install(TARGETS ...)`
* `install(FILES ...)`
The result is not just “the binary exists”, but “the consumer has what they need”.
---
## Exporting targets
Once targets are installed, another project may want to import them.
This is where exports come in.
For the install tree, the main mechanism is:
```cmake
install(EXPORT ...)
```
The documentation describes `install(EXPORT <export-name> ...)` as installing a CMake file exporting targets for dependent projects. It also shows the common pattern of associating targets with an export name via the `EXPORT` option of `install(TARGETS ...)`, and then installing the export file itself with `install(EXPORT ...)`. ([CMake][1])
That means the normal pattern is:
1. associate targets with an export set
2. install those targets
3. install the export file for consumers
---
## `export()` vs `install(EXPORT ...)`
This distinction is worth repeating because it is one of the most important conceptual points in the chapter.
### `export(TARGETS ...)`
For use directly from the **build tree**.
### `install(EXPORT ...)`
For use from the **install tree**.
The `export()` documentation says the file it creates is specific to the build tree and should never be installed, while `install(EXPORT ...)` is explicitly the install-tree export mechanism. ([CMake][2])
For this tutorial, the main focus should be the install-tree path because that is what most users need for a reusable installed package.
---
## Namespaces for exported targets
When exporting targets, it is common to add a namespace such as:
```cmake
install(EXPORT MyProjectTargets
    NAMESPACE MyProject::
    DESTINATION lib/cmake/MyProject
)
```
Both `export()` and `install(EXPORT ...)` support a `NAMESPACE` option that prepends a string to exported target names. ([CMake][2])
That gives consumers a clean usage style such as:
```cmake
target_link_libraries(app PRIVATE MyProject::my_lib)
```
which is a very common and readable CMake package pattern.
---
## Package config files
Exported targets are important, but a polished reusable package usually also installs:
* `MyProjectConfig.cmake`
* `MyProjectConfigVersion.cmake`
These files help a consuming project use `find_package(MyProject)` cleanly.
CMake provides the `CMakePackageConfigHelpers` module for this job. Its example shows using `configure_package_config_file()` together with `write_basic_package_version_file()`, then installing both files into a package directory. ([CMake][3])
This is the normal transition from:
* “I exported some targets”
  to:
* “another project can find my package”
---
## Package version files
A package version file helps consumers decide whether the installed package version is compatible with what they asked for.
CMake’s `CMakePackageConfigHelpers` module provides `write_basic_package_version_file()` for generating this file, and the documented example uses it to create `FooConfigVersion.cmake` alongside the package config file. ([CMake][3])
That fits very naturally with our earlier versioning chapters:
* the project already has a version
* now the installed package can expose version compatibility information too
---
## A likely chapter example
A good Chapter 11 example should probably contain:
* one reusable library
* one public header
* install rules for target and headers
* an export set
* a generated package config file
* a generated package version file
That keeps the chapter practical without trying to jump into every packaging detail at once.
A typical install layout might look something like:
```text
<prefix>/
├─ bin/
├─ lib/
├─ include/
└─ lib/cmake/MyProject/
```
That kind of layout is the natural outcome of relative install destinations such as `bin`, `lib`, `include`, and `lib/cmake/MyProject`. The install docs also show per-artifact destinations and components in their examples. ([CMake][1])
---
## Components
CMake install rules also support **components** such as:
* `Runtime`
* `Development`
The install documentation explains that install rules can be associated with a component via `COMPONENT`, and that component-specific installation can then be selected with `cmake --install --component`. It also notes that if no component is provided, a default component named `Unspecified` is created. ([CMake][1])
This is useful because many projects naturally split install content into:
* runtime artifacts for users
* headers and import/static artifacts for developers
We do not need to overcomplicate the first example with components, but they are worth introducing.
---
## Packaging with CPack
Once install rules exist, packaging becomes much easier.
CPack is the CMake packaging tool, and by default it reads packaging details from `CPackConfig.cmake` in the current directory. The `cpack` manual also documents the `-G` option for selecting one or more package generators and the `-R` option for overriding the package version. ([CMake][4])
That means the usual progression is:
1. define install rules
2. make the install/export layout correct
3. use CPack to generate distributable packages
This is why installation comes before packaging in the tutorial flow.
---
## Why packaging belongs in this chapter, but lightly
Packaging is important, but it should not dominate the chapter.
The first thing readers need is the structure:
* install tree
* exports
* config files
* version file
Then packaging becomes a natural next step rather than a mysterious extra layer.
A light first exposure is enough:
* explain what CPack is
* show where it fits
* leave deeper generator-specific packaging details for later or as optional expansion
---
## Good habits in this chapter
### 1. Use relative install destinations
That keeps install rules relocatable and CPack-friendly. The install docs explicitly recommend relative `DESTINATION` paths. ([CMake][1])
### 2. Separate build-tree export from install-tree export
`export()` and `install(EXPORT ...)` serve different purposes. ([CMake][2])
### 3. Install public headers deliberately
Do not assume the target alone is enough for consumers.
### 4. Use a namespace for exported targets
That makes consumer code much clearer. Both export mechanisms support `NAMESPACE`. ([CMake][2])
### 5. Keep package versioning aligned with the project version
That avoids drift between runtime versioning and package metadata. `write_basic_package_version_file()` is designed for this. ([CMake][3])
---
## Common mistakes
### 1. Installing to hardcoded absolute paths
That makes installation and packaging much less flexible. Relative destinations are the recommended style. ([CMake][1])
### 2. Confusing `export()` with `install(EXPORT ...)`
One is for the build tree, the other for the install tree. ([CMake][2])
### 3. Installing binaries but forgetting headers or config files
That leaves consumers without what they need.
### 4. Skipping the package version file
That weakens `find_package()`-style compatibility checks. `write_basic_package_version_file()` exists specifically to generate that file. ([CMake][3])
### 5. Treating packaging as disconnected from installation
CPack depends on the installation model and reads packaging configuration from `CPackConfig.cmake` by default. ([CMake][4])
---
## What this chapter intentionally keeps simple
This first install/export/package chapter does **not** need to cover everything.
We are intentionally keeping out:
* advanced component packaging
* platform-specific installer generators in depth
* dependency scanning during installation
* complex multi-package splits
* CPS package info details
* distributor-focused packaging edge cases
Those are real topics, but the first clean lesson is:
* install the right things
* export the right targets
* generate the right package config files
* understand where CPack fits
---
## This is enough for now
At this stage, the key ideas to keep are:
* `install()` defines installation rules for targets and files
* relative destinations are the best default
* `export()` is for build-tree consumption
* `install(EXPORT ...)` is for install-tree consumption
* package config and version files complete the reusable package story
* CPack builds on top of a correct install layout
That is the mental model we want before writing the code. ([CMake][1])
---
## Takeaways
This chapter is where the project starts to become reusable beyond the current build directory.
We are moving from:
* “the target builds here”
to:
* “the target installs cleanly”
* “other projects can import it”
* “the project can be packaged for distribution”
That is a major milestone in understanding real CMake project structure.
---
## Next step in this chapter
The code for this chapter should define:
* one small reusable library
* one public header
* install rules for runtime, library, and headers
* an export set
* a generated package config file
* a generated package version file
* a basic CPack setup

[1]: https://cmake.org/cmake/help/latest/command/install.html "install — CMake 4.3.2 Documentation"
[2]: https://cmake.org/cmake/help/latest/command/export.html "export — CMake 4.3.2 Documentation"
[3]: https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html "CMakePackageConfigHelpers — CMake 4.3.2 Documentation"
[4]: https://cmake.org/cmake/help/latest/manual/cpack.1.html "cpack(1) — CMake 4.3.2 Documentation"
