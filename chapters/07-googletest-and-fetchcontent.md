# Chapter 07 - GoogleTest and External Dependencies with FetchContent
**Note If FetchContent download fails in restricted or corporate environments, the cause is often TLS or certificate validation rather than a problem in the CMake logic itself.In those cases, using a vendored local copy of GoogleTest is often the most reliable approach for tutorials and training.**
In the previous chapter we introduced CTest and the basic CMake testing model:
- build a test executable
- register it with `add_test()`
- run it with `ctest`
That gave us the core idea.
Now we take the next step:
we will use a real testing framework and bring it into the project from an external source.
This chapter introduces two very practical topics at once:
- **GoogleTest**
- **external dependencies with `FetchContent`**
These are both common in real CMake-based projects.
---
## Why this matters
Most real projects depend on code that is not written in the same repository.
Examples:
- testing frameworks
- logging libraries
- serialization libraries
- networking libraries
- formatting libraries
- utility/header-only packages
So at some point every team has to answer:
- how do we get dependencies into the build?
- how do we keep the build reproducible?
- how do we connect those dependencies cleanly to our targets?
This chapter gives the first practical answer.
We will use GoogleTest because it is:
- widely known
- easy to demonstrate
- immediately useful
- a good example of a real external dependency
---
## What we will cover
In this chapter we will cover:
- what GoogleTest adds on top of plain CTest
- why `FetchContent` is useful
- how to fetch GoogleTest from the network
- how to link GoogleTest to a test target
- how to register tests with CTest
- how framework-based tests differ from hand-written pass/fail executables
We will also briefly position:
- `find_package()`
- `FetchContent`
- `ExternalProject`
so the reader sees where `FetchContent` fits.
---
## The key idea
In Chapter 06, our test executable manually checked values and returned:
- `0` for success
- non-zero for failure
That works and is still useful.
But a testing framework gives us more:
- assertions
- clearer test structure
- grouped test cases
- better failure reporting
- easier scaling as the test suite grows
GoogleTest is one of the standard ways to get those benefits in C++ projects.
---
## Why this chapter also covers external dependencies
Because GoogleTest usually does not come from nowhere.
We need to bring it into the project somehow.
That is exactly what makes this a good dependency chapter:
we are not discussing dependencies in the abstract,
we are using one for a real purpose.
So this chapter is not just:
- “here is GoogleTest”
It is also:
- “here is one practical way to add an external dependency to a CMake project”
---
## Three common dependency approaches
When people talk about dependencies in CMake, three names come up often:
- `find_package()`
- `FetchContent`
- `ExternalProject`
We are not going to go deep into all three right now, but it is useful to place them.
### `find_package()`
Use this when the dependency is already installed or provided by the environment.
This is common for:
- system packages
- SDKs
- libraries installed by a package manager
- dependencies provided by a toolchain
### `FetchContent`
Use this when you want CMake to download and make the dependency available as part of the configure/build flow.
This is often very convenient for:
- tutorials
- demos
- small to medium projects
- tightly controlled dependency versions
### `ExternalProject`
Use this when you want a more isolated “build another project separately” style.
This can be useful, but it is heavier and not the best first teaching path for this repo.
For this chapter, `FetchContent` is the right tool.
---
## Why `FetchContent` is a good first dependency mechanism
`FetchContent` is very attractive for a tutorial because it is:
- direct
- readable
- self-contained
- easy to reproduce
- simple to connect to targets
That means the repo can show a dependency and its usage in one place without requiring the reader to install everything manually first.
It is also good for teaching because it makes dependency use visible in CMake itself.
---
## What we will build
We will build:
- a small library target: `core_math`
- a GoogleTest-based test executable: `core_math_gtests`
The tests will verify things like:
- `add(2, 3) == 5`
- `multiply(4, 5) == 20`
Then we will integrate that executable with CTest.
So the final flow will be:
1. configure the project
2. fetch GoogleTest
3. build the library and tests
4. run tests with `ctest`
---
## Folder layout
Example folder:
```text
examples/
└─ 07-googletest-and-fetchcontent/
   ├─ CMakeLists.txt
   ├─ src/
   │  ├─ math_utils.cpp
   │  └─ math_utils.h
   └─ tests/
      └─ test_math.cpp
````
This is intentionally similar to Chapter 06.
That is useful because the reader can compare:
* plain CTest test executable
* GoogleTest-based test executable
without having to learn a completely different project shape at the same time.
---
## What changes compared to Chapter 06
In Chapter 06:
* we wrote our own mini test checks
* we printed pass/fail messages ourselves
* we returned non-zero manually if a check failed
In this chapter:
* GoogleTest provides the assertion macros
* tests are organized into named test cases
* the framework reports detailed failures
* we still run everything through CTest
So the structure becomes more scalable and expressive.
---
## What GoogleTest gives us
A framework like GoogleTest gives us a better vocabulary for tests.
Instead of writing custom check helpers everywhere, we can write things like:
```cpp
EXPECT_EQ(add(2, 3), 5);
EXPECT_EQ(multiply(4, 5), 20);
```
That is:
* shorter
* more readable
* easier to extend
* easier to debug when a failure happens
This is one of the big reasons teams adopt a real test framework.
---
## The role of CTest does not go away
One very important thing to understand:
GoogleTest does **not** replace CTest in the overall CMake workflow.
Instead:
* GoogleTest provides test structure and assertions
* CMake builds the test executable
* CTest still runs the registered tests
That is why Chapter 06 had to come first.
CTest is still the outer execution model.
---
## The high-level CMake shape
The chapter’s `CMakeLists.txt` will roughly do these things:
1. enable testing
2. include `FetchContent`
3. declare GoogleTest as a fetched dependency
4. make it available
5. build our library target
6. build a GoogleTest-based test executable
7. link it to `GTest::gtest_main`
8. register the tests with CTest
That is the full dependency-to-test flow we want the reader to see.
---
## Why we pin the dependency version
When fetching external code, it is good practice to pin to a known version.
That helps with:
* reproducibility
* debugging
* stability
* consistency across machines
In a tutorial repo, pinning is especially important because we want the example to behave consistently over time.
So we will fetch a specific GoogleTest release rather than just “whatever is latest today.”
---
## A likely CMake outline
The actual code will come next, but conceptually it will look something like this:
```cmake id="0aiwe2"
include(FetchContent)
FetchContent_Declare(
    googletest
    ...
)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_library(core_math ...)
add_executable(core_math_gtests ...)
target_link_libraries(core_math_gtests PRIVATE core_math GTest::gtest_main)
add_test(NAME core_math_gtests COMMAND core_math_gtests)
```
That is the basic shape.
There are other valid ways to integrate GoogleTest, but this is a very good first path.
---
## Why `FetchContent` is nice for this repo
For our tutorial, `FetchContent` has several advantages:
### 1. The dependency story is visible
Readers can see exactly how the dependency enters the project.
### 2. The example is more self-contained
They do not have to install GoogleTest manually before trying the chapter.
### 3. It fits the chapter topic naturally
The dependency is not abstract.
It is immediately used for something practical.
### 4. It sets up later discussions
Later, if we want, we can compare this with:
* `find_package()`
* package managers
* vendoring
* offline/corporate builds
So this chapter becomes the foundation, not the final word.
---
## A useful practical warning
Fetching from the network is convenient, but it is not always the right final production policy.
Some teams prefer:
* vendored dependencies
* package managers
* internal mirrors
* preinstalled packages
* offline reproducible flows
That is okay.
This chapter is teaching the **first practical mechanism**, not claiming it is the only one for every environment.
For a tutorial repo, the convenience of `FetchContent` is worth it.
---
## What the tests will look like
The GoogleTest version of the tests will likely look something like this in spirit:
* one test for `add`
* one test for `multiply`
* one or two additional cases for negatives or zero
That gives us:
* a clearer test structure than the previous chapter
* readable assertion output
* a good comparison against the plain CTest approach
---
## What the reader should notice
When we get to the code, there are a few important things to notice:
### 1. The production code barely changes
The library can stay simple.
### 2. The test code becomes more expressive
Assertions and test grouping come from GoogleTest.
### 3. The dependency is attached through targets
We still link things target-to-target.
### 4. CTest is still the runner
We are not leaving the CMake testing model.
Those four points are the real lesson.
---
## Configure, build, and test flow
The workflow will still feel familiar:
### Configure
```bash id="6rmb9n"
cmake -S examples/07-googletest-and-fetchcontent -B build/07-googletest-and-fetchcontent
```
### Build
```bash id="c1hvuk"
cmake --build build/07-googletest-and-fetchcontent
```
### Run tests
```bash id="z0qz7z"
ctest --test-dir build/07-googletest-and-fetchcontent --output-on-failure
```
That is very intentional.
We want the chapter to feel like a natural extension of Chapter 06, not like a completely different world.
---
## Why this chapter belongs in the main flow
This topic is too important to hide in an appendix.
External dependencies are normal project reality.
So Chapter 07 should treat them as first-class material:
* fetching a real dependency
* using it for a real purpose
* integrating it cleanly into CMake
That is exactly the kind of practical skill the tutorial should teach.
An appendix can still later discuss:
* advanced dependency strategy
* corporate mirrors
* package managers
* vendoring vs fetching
* offline builds
But the first real dependency example belongs here.
---
## Common mistakes we want this chapter to prevent
### 1. Treating framework tests as unrelated to CTest
They still fit into the same CMake testing workflow.
### 2. Fetching a dependency without pinning it
That makes builds less reproducible.
### 3. Linking dependencies in a non-target-based way
We want to keep the target-oriented style from earlier chapters.
### 4. Overcomplicating the first dependency example
The goal is not to teach every dependency pattern at once.
### 5. Confusing “downloaded during configure” with “always the best policy”
It is useful, but context matters.
---
## What this chapter intentionally keeps simple
We are intentionally not covering all of these yet:
* `find_package()` in depth
* `ExternalProject`
* package managers
* internal dependency mirrors
* offline environments
* dependency caching policy
* advanced GoogleTest discovery features
* large multi-target dependency graphs
Those are all real topics, but the first clean win is:
* fetch one dependency
* use it successfully
* understand how it fits the target model
---
## This is enough for now
At this stage, the key ideas to keep are:
* GoogleTest gives us a real C++ testing framework
* `FetchContent` is a convenient way to bring in an external dependency
* the dependency should still be integrated through targets
* CTest remains the outer test runner
* this chapter is both a testing chapter and a dependency chapter
That is the mental model we need before writing the code.
---
## Takeaways
This chapter is where the tutorial starts to feel like a real project ecosystem.
We move from:
* “one project, one repo, all code local”
to:
* “real targets”
* “real framework”
* “real external dependency”
We are still keeping it small and readable, but the workflow is now much closer to everyday engineering work.
---
## Next step in this chapter
The code for this chapter should define:
* a small library under test
* a `FetchContent` block for GoogleTest
* a GoogleTest-based test executable
* CMake linking against the GoogleTest targets
* CTest integration for running the tests
That is the code we should write next.
---
## Next chapter
In the next chapter we will shift from testing and dependencies toward build-time tooling.
That is where we will cover:
* external scripts
* generated files
* custom commands
* and connecting generated outputs into the build
```
```
