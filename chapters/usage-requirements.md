# Chapter 04 - Usage Requirements: PRIVATE, PUBLIC, and INTERFACE
In the previous chapter we built a DLL and loaded it dynamically from a separate executable.
Now we step back and focus on one of the most important ideas in modern CMake:
**usage requirements**.
This is the chapter where target-based CMake starts to become much clearer.
A lot of older or messy CMake code spreads settings around globally:
- include directories
- compile definitions
- compiler flags
- link dependencies
Modern CMake tries to do something better:
attach those requirements to targets, and describe how they should flow to other targets.
That is what `PRIVATE`, `PUBLIC`, and `INTERFACE` are for.
---
## Why this matters
Once a project has more than one target, a big question appears:
> which settings belong only to this target, and which settings should also be visible to targets that use it?
That question applies to:
- include directories
- compile definitions
- compile options
- linked libraries
- compile features
If this is modeled badly:
- consumers miss include paths
- builds break in confusing ways
- targets become harder to reuse
- global settings start leaking everywhere
If this is modeled well:
- each target describes its own needs
- consumers automatically receive what they need
- projects scale much more cleanly
That is why this chapter matters so much.
---
## The core idea
A target can have requirements.
Some requirements are only needed to build that target itself.
Some requirements are also needed by anything that uses that target.
CMake uses three keywords to describe that:
- `PRIVATE`
- `PUBLIC`
- `INTERFACE`
You will see these most often in commands like:
- `target_include_directories()`
- `target_compile_definitions()`
- `target_compile_options()`
- `target_link_libraries()`
---
## The shortest practical meaning
A good working mental model is:
### `PRIVATE`
Used only when building this target.
### `PUBLIC`
Used when building this target, and also passed to targets that link to it.
### `INTERFACE`
Not used to build this target itself, but passed to targets that link to it.
That is the whole chapter in one paragraph.
Everything else is learning how to apply that idea correctly.
---
## Why the names can feel strange at first
These words are not about "visibility" in the C++ language sense.
They are about **how build requirements propagate**.
A target can expose usage requirements to other targets.
So the question is not:
- "is this variable public?"
The real question is:
- "does a consumer of this target also need this build information?"
That framing makes the keywords much easier to understand.
---
## First example: include directories
Suppose we have a library target called `math_lib` and its public header is included by other targets.
If the consumer will write:
```cpp
#include "math_api.h"
````
then the consumer needs the include path.
That means the include directory should usually be:
```cmake
target_include_directories(math_lib PUBLIC some/include/path)
```
Why `PUBLIC`?
Because:
* the library itself needs the header path when it builds
* anything using the library also needs the header path
That is a classic `PUBLIC` case.
---
## What would be `PRIVATE` for include directories?
Suppose the library source file includes an internal helper header that consumers never see.
For example:
* internal implementation headers
* private platform wrappers
* source-only helper headers
Those paths are only needed when building the library itself.
That makes them a good `PRIVATE` case.
Example idea:
```cmake
target_include_directories(math_lib PRIVATE src/internal)
```
Consumers should not need to know about those internals.
---
## What would be `INTERFACE` for include directories?
This is less common at first, but very useful.
An `INTERFACE` include directory is not needed to build the target itself, but consumers need it.
This often appears with:
* header-only libraries
* pure interface targets
* abstraction targets that do not compile sources themselves
For example, a header-only utility target might use:
```cmake
target_include_directories(my_headers INTERFACE include)
```
There is no compilation for the target itself, but consumers need the headers.
---
## The same idea with compile definitions
The same logic applies to compile definitions.
### Example: `PRIVATE`
If a compile definition is only used internally by the target implementation:
```cmake
target_compile_definitions(math_lib PRIVATE MATHLIB_INTERNAL_BUILD=1)
```
Then consumers do not need it.
### Example: `PUBLIC`
If a compile definition changes how consumers include or use the target API:
```cmake
target_compile_definitions(math_lib PUBLIC USE_FAST_MATH_API=1)
```
Then both the target and its consumers may need it.
### Example: `INTERFACE`
If the definition is only relevant to users of the target, not to the target build itself:
```cmake
target_compile_definitions(my_headers INTERFACE MY_HEADERS_ENABLED=1)
```
Then `INTERFACE` is the right model.
---
## The same idea with linked libraries
This one is especially important.
Suppose `app` links to `math_lib`:
```cmake
target_link_libraries(app PRIVATE math_lib)
```
That means:
* `app` uses `math_lib`
* but the dependency does not automatically become part of `app`'s outward interface to others
Now imagine `math_lib` itself depends on another library, say `core_types`.
If users of `math_lib` also need `core_types` in order to compile or link correctly, then that dependency may need to be:
```cmake
target_link_libraries(math_lib PUBLIC core_types)
```
But if `core_types` is only an implementation detail of `math_lib`, then:
```cmake
target_link_libraries(math_lib PRIVATE core_types)
```
is more appropriate.
So again, the question is:
> Is this dependency part of the outward usage requirements of the target?
If yes, it may be `PUBLIC` or `INTERFACE`.
If no, it is likely `PRIVATE`.
---
## Why this chapter follows Chapter 03
In Chapter 03 we built a DLL and a loader EXE.
That chapter focused on:
* building separate targets
* exporting symbols
* runtime loading
This chapter focuses on a more general design rule:
how targets describe what they require from consumers.
Even in the DLL example, this matters.
For example:
* include paths for public headers
* compile definitions for export/import handling
* internal-only vs consumer-facing settings
So this chapter is the conceptual cleanup that makes later projects much more maintainable.
---
## A useful mental question
Whenever you write one of these commands, ask:
> Is this only needed to build this target, or do consumers need it too?
That question solves most `PRIVATE` / `PUBLIC` / `INTERFACE` decisions.
A second useful question is:
> Is this target even compiled itself?
If the target is header-only or interface-only, then `INTERFACE` often becomes the natural choice.
---
## A simple target-oriented example
Imagine this structure:
* `core_math` — a library
* `app` — an executable using it
Now suppose:
* `core_math` has public headers under `include/`
* internal helper headers under `src/`
* an internal implementation define
* a public API define
A CMake sketch might look like:
```cmake
add_library(core_math ...)
target_include_directories(core_math
    PUBLIC  include
    PRIVATE src
)
target_compile_definitions(core_math
    PUBLIC  CORE_MATH_API_LEVEL=2
    PRIVATE CORE_MATH_INTERNAL_BUILD=1
)
add_executable(app ...)
target_link_libraries(app PRIVATE core_math)
```
The meaning is:
* `include/` is needed by `core_math` and by consumers
* `src/` is only needed to build `core_math`
* `CORE_MATH_API_LEVEL=2` affects users too
* `CORE_MATH_INTERNAL_BUILD=1` is only for internal implementation
* `app` uses `core_math`, but `app` does not automatically re-expose it as part of its own outward interface
That is the pattern we want readers to get comfortable with.
---
## The difference between target properties and global settings
Older CMake often uses global commands like:
* `include_directories(...)`
* `add_definitions(...)`
* broad flag variables
Those approaches can work, but they scale badly because they make it harder to see:
* which target needs what
* what is part of the public API
* what is implementation detail
* how requirements flow through the dependency graph
Modern CMake prefers:
* `target_include_directories(...)`
* `target_compile_definitions(...)`
* `target_compile_options(...)`
* `target_link_libraries(...)`
This is one of the biggest style improvements in contemporary CMake practice.
---
## `PRIVATE`, `PUBLIC`, and `INTERFACE` as propagation rules
Another way to think about them is as propagation rules.
### `PRIVATE`
Do not propagate.
### `PUBLIC`
Propagate to consumers, and also use locally.
### `INTERFACE`
Propagate to consumers only.
This “propagation” phrasing is often easier to remember than the longer technical explanation.
---
## Header-only targets and `INTERFACE` libraries
A very natural place to use `INTERFACE` is a header-only library.
For example:
```cmake
add_library(math_headers INTERFACE)
target_include_directories(math_headers INTERFACE include)
target_compile_features(math_headers INTERFACE cxx_std_17)
```
This says:
* there is no compiled library artifact
* consumers should get the include path
* consumers should get the compile feature requirement
That is a very clean modern CMake pattern.
We are not building a full header-only example in this chapter, but it is worth knowing because it shows why `INTERFACE` exists.
---
## How this connects to export macros
Consider the export/import macro pattern from the DLL chapter.
A definition like:
* `BUILDING_MATH_DLL`
is normally only relevant while building the DLL itself.
That makes it a good candidate for:
```cmake
target_compile_definitions(math_dll PRIVATE BUILDING_MATH_DLL)
```
Consumers should not build as though they are the DLL itself.
That is a perfect example of why `PRIVATE` matters.
On the other hand, if there were a definition that consumers must see for the public API to behave correctly, that might be `PUBLIC`.
This shows that usage requirements are not abstract theory.
They directly affect practical DLL design.
---
## A common mistake: making everything PUBLIC
One of the most common mistakes is to overuse `PUBLIC`.
That can make the build “work,” but it leaks too much information to consumers:
* internal include paths
* implementation defines
* flags that should stay private
* accidental dependencies
This creates a messy outward interface.
A good rule is:
* start with `PRIVATE`
* use `PUBLIC` only when consumers truly need the requirement
* use `INTERFACE` when the requirement is only for consumers
That tends to lead to cleaner designs.
---
## Another common mistake: making everything PRIVATE
The opposite mistake also happens.
If public headers require a path or definition, but you mark it `PRIVATE`, then consumers fail later with confusing errors:
* header not found
* missing compile definition
* unresolved symbols
* mismatched configuration
So `PRIVATE` is not “safer” by default.
It is only correct when the requirement is truly internal.
---
## Reading CMake through target interfaces
A very useful advanced habit is to read a CMake project by asking:
1. What are the targets?
2. What does each target expose?
3. What does each target keep private?
4. Which requirements flow transitively to consumers?
This is much more powerful than reading the project as a flat list of commands.
It lets you reason about:
* build structure
* dependency boundaries
* public API shape
* reuse potential
That is one reason this chapter is such an important turning point.
---
## Practical example decisions
Here are some common cases and their likely answers.
### Public headers folder
Usually `PUBLIC`
### Internal source-only headers
Usually `PRIVATE`
### Internal implementation define
Usually `PRIVATE`
### Header-only compile requirement
Usually `INTERFACE`
### Library dependency needed by public headers
Often `PUBLIC`
### Library dependency only used inside source files
Usually `PRIVATE`
These are not laws of nature, but they are very good defaults.
---
## What this chapter intentionally does not do
This chapter explains the concepts, but keeps a few things for later:
* installation interface paths
* generator expressions for build vs install include paths
* exported targets
* package config propagation
* deeper transitive dependency examples
Those are important, but the core concept has to be clear first.
---
## A compact cheat sheet
### `PRIVATE`
Needed only to build this target.
### `PUBLIC`
Needed to build this target and also needed by its consumers.
### `INTERFACE`
Not needed to build this target itself, but needed by its consumers.
That is the whole rule.
---
## Common mistakes
### 1. Using global include or definition commands everywhere
This makes target relationships harder to understand.
### 2. Marking implementation details as `PUBLIC`
This leaks internal structure to consumers.
### 3. Marking true usage requirements as `PRIVATE`
This breaks consumers.
### 4. Forgetting that `INTERFACE` is useful for header-only targets
It is not just an obscure edge case.
### 5. Choosing a keyword without asking who actually needs the requirement
That usually leads to guesswork and messy builds.
---
## This is enough for now
At this point, the main things to remember are:
* usage requirements belong to targets
* `PRIVATE`, `PUBLIC`, and `INTERFACE` describe propagation
* the key question is whether consumers also need the requirement
* modern CMake becomes much clearer when read as target interfaces
That is enough foundation for the next steps.
---
## Takeaways
This chapter is the heart of modern target-based CMake.
We learned that:
* targets can carry build requirements
* some requirements stay local
* some requirements are part of the target’s outward interface
* CMake provides `PRIVATE`, `PUBLIC`, and `INTERFACE` to model that cleanly
Once this idea clicks, a lot of CMake code becomes much easier to design and read.
---
## Next chapter
In the next chapter we will move into build configurations and organize them more explicitly.
That is where we will cover:
* Debug and Release
* configuration-specific behavior
* and custom build flavors such as `SimDebug`
```
