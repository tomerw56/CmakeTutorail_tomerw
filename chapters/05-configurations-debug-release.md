# Chapter 05 - Configurations: Debug, Release, and Custom Flavors
In the previous chapter we focused on usage requirements:
- `PRIVATE`
- `PUBLIC`
- `INTERFACE`
Now we move to a different but equally practical topic:
**build configurations**.
This chapter explains how to think about:
- `Debug`
- `Release`
- `RelWithDebInfo`
- `MinSizeRel`
- and custom team-specific flavors such as `SimDebug`
This is where we connect the earlier generator discussion to real project behavior.
CMake treats configuration differently depending on the generator:
single-config generators use `CMAKE_BUILD_TYPE`, while multi-config generators use `CMAKE_CONFIGURATION_TYPES`. Both support custom configuration names. :contentReference[oaicite:0]{index=0}
---
## Why this matters
Many teams eventually need more than just “Debug or Release”.
Examples:
- a simulation-focused debug build
- a diagnostics build
- a debug build with extra tracing
- a release-like build with symbols
- a CI-specific flavor
A common first instinct is to scatter flags and definitions around manually.
That usually becomes hard to maintain.
A better approach is to understand:
- what a configuration is
- when configuration is selected
- how to express config-specific behavior cleanly
- when a custom configuration is appropriate
- when a normal option is better than inventing a new configuration
---
## What we will cover
In this chapter we will cover:
- standard configuration names
- single-config vs multi-config behavior
- `CMAKE_BUILD_TYPE`
- `CMAKE_CONFIGURATION_TYPES`
- config-specific compile definitions and options
- custom flavors such as `SimDebug`
- when to use a custom config vs a separate project option
- why presets are often a good companion for team workflows
CMake’s docs describe `CMAKE_BUILD_TYPE` as the build type selector for single-config generators and `CMAKE_CONFIGURATION_TYPES` as the list of available configurations for multi-config generators; both can use custom names. :contentReference[oaicite:1]{index=1}
---
## Standard configuration names
The standard names most people encounter are:
- `Debug`
- `Release`
- `RelWithDebInfo`
- `MinSizeRel`
These are common conventions, not the only possible names.
CMake explicitly documents these as typical values, and also notes that custom build types/configurations can be defined. :contentReference[oaicite:2]{index=2}
A practical mental model is:
### `Debug`
Built for development and debugging.
### `Release`
Built for optimized delivery-oriented use.
### `RelWithDebInfo`
Release-style optimization with debug information.
### `MinSizeRel`
Optimized for smaller size.
The exact flags behind these depend on the compiler, toolchain, and generator, so they are conventions with platform-specific implementation details. CMake’s buildsystem manual also notes that configuration names are treated case-insensitively in many contexts, though conventional capitalization is strongly preferred. :contentReference[oaicite:3]{index=3}
---
## The core split: single-config vs multi-config
This is the foundation of the whole chapter.
### Single-config generators
Examples:
- `Ninja`
- `Unix Makefiles`
These usually select one build type per build directory.
That is why commands like this are normal:
```bash
cmake -S . -B build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
````
or:
```bash
cmake -S . -B build-simdebug -G Ninja -DCMAKE_BUILD_TYPE=SimDebug
cmake --build build-simdebug
```
CMake documents `CMAKE_BUILD_TYPE` as the variable that specifies the build type on single-configuration generators, and it explicitly says custom build types can also be defined. ([CMake][1])
---
### Multi-config generators
Examples:
* `Visual Studio`
* `Xcode`
* `Ninja Multi-Config`
These generate support for multiple configurations in the same build tree.
Typical pattern:
```bash
cmake -S . -B build-vs -G "Visual Studio 17 2022"
cmake --build build-vs --config Debug
cmake --build build-vs --config Release
```
For multi-config generators, the available configuration names come from `CMAKE_CONFIGURATION_TYPES`. CMake’s Ninja Multi-Config docs also note that it uses `CMAKE_CONFIGURATION_TYPES` instead of `CMAKE_BUILD_TYPE`. ([CMake][2])
---
## `CMAKE_BUILD_TYPE`
For single-config generators, `CMAKE_BUILD_TYPE` is the main configuration selector.
Example:
```bash
cmake -S examples/05-configurations-debug-release -B build/05-ninja-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
```
Another example with a custom flavor:
```bash
cmake -S examples/05-configurations-debug-release -B build/05-ninja-simdebug -G Ninja -DCMAKE_BUILD_TYPE=SimDebug
```
This works because CMake allows custom build types for single-config generators. ([CMake][1])
A useful practical takeaway is:
* one build folder
* one selected build type
That is why names like:
* `build-debug`
* `build-release`
* `build-simdebug`
are good habits for single-config workflows.
---
## `CMAKE_CONFIGURATION_TYPES`
For multi-config generators, the available configuration names are defined as a list.
Example:
```cmake
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;SimDebug" CACHE STRING "" FORCE)
```
This is how you make a custom configuration such as `SimDebug` available to generators like Visual Studio or Ninja Multi-Config. CMake documents `CMAKE_CONFIGURATION_TYPES` as the semicolon-separated list of available build configurations and says custom build types can be defined there too. ([CMake][2])
After that, builds look like:
```bash
cmake -S examples/05-configurations-debug-release -B build/05-vs -G "Visual Studio 17 2022"
cmake --build build/05-vs --config SimDebug
```
That is the multi-config equivalent of the single-config `CMAKE_BUILD_TYPE=SimDebug` flow. ([CMake][2])
---
## Our custom flavor: `SimDebug`
For this tutorial, we will use:
```text
SimDebug
```
The intent is:
* it behaves like a debug-oriented build
* but it also enables simulator-specific behavior
This is a good example of a custom configuration because it represents a repeated, meaningful team workflow rather than a one-off tweak.
CMake supports custom configuration names, so `SimDebug` is a valid configuration name for both single-config and multi-config workflows. ([CMake][1])
---
## The clean way to express config-specific behavior
A modern target-based way to express configuration behavior is with generator expressions.
For example:
```cmake
target_compile_definitions(my_app PRIVATE
    $<$<CONFIG:Debug>:APP_DEBUG=1>
    $<$<CONFIG:Release>:APP_RELEASE=1>
    $<$<CONFIG:SimDebug>:APP_SIM_DEBUG=1>
)
```
Generator expressions are evaluated during buildsystem generation and can produce configuration-specific values. The generator-expressions manual documents `$<...>` syntax and configuration-aware evaluation. ([CMake][3])
This is usually the right first teaching path because it is:
* target-based
* local
* explicit
* easier to reason about than broad global flag mutation
---
## A good Chapter 05 pattern
For this tutorial, a practical pattern is:
```cmake
add_executable(config_demo main.cpp)
target_compile_definitions(config_demo PRIVATE
    $<$<CONFIG:Debug>:APP_CONFIG_NAME="Debug">
    $<$<CONFIG:Release>:APP_CONFIG_NAME="Release">
    $<$<CONFIG:RelWithDebInfo>:APP_CONFIG_NAME="RelWithDebInfo">
    $<$<CONFIG:MinSizeRel>:APP_CONFIG_NAME="MinSizeRel">
    $<$<CONFIG:SimDebug>:APP_CONFIG_NAME="SimDebug">
    $<$<CONFIG:SimDebug>:ENABLE_SIMULATION=1>
)
```
This makes the active configuration visible in the program and also toggles simulator behavior for `SimDebug`.
That gives us a very readable and very demo-friendly result.
---
## Can we define custom flags globally too?
Yes, but this should not be the first thing we teach.
There are older, more global ways to shape configuration behavior, such as config-specific flag variables.
Those patterns are still seen in real codebases, but for this tutorial the cleaner route is:
* first teach config-specific target behavior
* then mention global config flags as a secondary technique
Why?
Because Chapter 04 already moved us toward target-oriented CMake, and Chapter 05 should keep reinforcing that style.
---
## Single-config workflow with `SimDebug`
For Ninja or Makefiles, the pattern is:
```bash
cmake -S examples/05-configurations-debug-release -B build/05-simdebug -G Ninja -DCMAKE_BUILD_TYPE=SimDebug
cmake --build build/05-simdebug
```
This is valid because custom build types are supported in `CMAKE_BUILD_TYPE`. ([CMake][1])
The executable can then print:
* which configuration it believes it was built under
* whether simulation mode was enabled
That makes the configuration visible and testable.
---
## Multi-config workflow with `SimDebug`
For Visual Studio or Ninja Multi-Config, we first make `SimDebug` available:
```cmake
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel;SimDebug" CACHE STRING "" FORCE)
```
Then:
```bash
cmake -S examples/05-configurations-debug-release -B build/05-vs -G "Visual Studio 17 2022"
cmake --build build/05-vs --config SimDebug
```
CMake’s docs say multi-config generators use `CMAKE_CONFIGURATION_TYPES`, and Ninja Multi-Config does this explicitly instead of `CMAKE_BUILD_TYPE`. ([CMake][2])
---
## When should a team create a custom configuration?
A custom configuration is a good fit when it represents a real, repeated build mode.
Good examples:
* `SimDebug`
* `Profile`
* `ASanDebug`
* `Coverage`
Less good examples:
* one feature toggle used by only one developer once
* a temporary experiment
* something that is really just a runtime option
* something that could be a normal CMake option
A useful question is:
> Is this a true build mode, or just one extra project feature?
If it is really a build mode, a custom configuration may make sense.
If it is just a feature, an option is often cleaner.
---
## Custom configuration vs normal option
This distinction matters a lot.
### Use a custom configuration when:
* it changes the character of the build
* it is used repeatedly by the team
* it belongs conceptually next to Debug/Release-style choices
### Use a normal option when:
* it toggles one feature
* it is orthogonal to Debug/Release
* you may want it on or off in multiple configurations
For example, this is often cleaner than inventing a new configuration:
```cmake
option(ENABLE_SIMULATION "Enable simulator-specific behavior" OFF)
target_compile_definitions(my_app PRIVATE
    $<$<BOOL:${ENABLE_SIMULATION}>:ENABLE_SIMULATION=1>
)
```
This lets you have:
* `Debug + simulation`
* `Release + simulation`
* `RelWithDebInfo + simulation`
without multiplying configuration names unnecessarily.
---
## Why presets are often a better team experience
Even when custom configurations are valid, teams often benefit from CMake Presets.
Presets exist to let users share common configure settings for repeated workflows and CI. CMake’s presets manual describes `CMakePresets.json` and `CMakeUserPresets.json` as the mechanism for sharing common configure options. ([CMake][4])
That means a team can define named flows such as:
* `ninja-debug`
* `ninja-simdebug`
* `vs2022-debug`
* `vs2022-simdebug`
without relying on everyone to remember the exact configure command.
A good practical approach is often:
* use configurations for true build modes
* use presets to standardize how those modes are invoked
---
## A practical teaching strategy for this repo
For our Chapter 05 example, the cleanest teaching path is:
### 1. Show standard Debug and Release
Make sure the reader sees the baseline.
### 2. Add `SimDebug`
Show that custom configuration names are possible.
### 3. Use generator expressions
Make the target react to the active config.
### 4. Mention options
Explain that not every special case deserves its own configuration.
### 5. Mention presets
Show how teams can standardize repeated flows.
That keeps the chapter practical without making it messy.
---
## A likely demo behavior
A good Chapter 05 executable could print:
* active config name
* whether simulation mode is enabled
* whether `NDEBUG` is active
* maybe whether a config-specific trace define is enabled
That would let the reader compare:
* Debug
* Release
* SimDebug
with visible runtime output.
---
## Common mistakes
### 1. Using `CMAKE_BUILD_TYPE` with Visual Studio and expecting it to drive everything
For multi-config generators, the active build configuration is usually chosen with `--config`, and the set of available configs comes from `CMAKE_CONFIGURATION_TYPES`. ([CMake][2])
### 2. Reusing the same build directory for conflicting generator/config workflows
Keep build trees clear and intentional.
### 3. Creating too many custom configurations
This makes the build harder to understand and harder to maintain.
### 4. Using a custom configuration when a simple option would do
Not every feature toggle should become a new configuration name.
### 5. Hard-coding behavior globally when the target-based form is clearer
Generator expressions on targets are usually easier to teach and maintain than broad global flag mutation. ([CMake][3])
---
## This is enough for now
At this point, the main ideas to keep are:
* single-config generators use `CMAKE_BUILD_TYPE`
* multi-config generators use `CMAKE_CONFIGURATION_TYPES`
* custom configuration names such as `SimDebug` are supported
* config-specific target behavior is best expressed clearly and locally
* not every special case should become a new configuration
* presets are often the cleanest way to standardize repeated team flows
That is the core mental model for configuration work in CMake. ([CMake][1])
---
## Takeaways
This chapter is where configurations stop being abstract labels and become practical design tools.
We learned that:
* `Debug` and `Release` are common defaults, not the only possibilities
* `SimDebug` is a valid custom build flavor
* the way you select configurations depends on the generator class
* generator expressions are a clean way to make targets react to configurations
* options and presets are often part of the same overall solution
That gives us a clean foundation for the Chapter 05 code.
---
## Next step in this chapter
The code for this chapter should demonstrate:
* standard config output
* custom `SimDebug`
* config-specific compile definitions
* single-config and multi-config command examples
* clear runtime prints showing which configuration was actually built
That is the code we should write next.

[1]: https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html?utm_source=chatgpt.com "CMAKE_BUILD_TYPE — CMake 4.3.1 Documentation"
[2]: https://cmake.org/cmake/help/latest/variable/CMAKE_CONFIGURATION_TYPES.html?utm_source=chatgpt.com "CMAKE_CONFIGURATION_TYP..."
[3]: https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html?utm_source=chatgpt.com "cmake-generator-expressions(7)"
[4]: https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html?utm_source=chatgpt.com "cmake-presets(7)"
