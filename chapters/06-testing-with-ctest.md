# Chapter 06 - Testing with CTest
In the previous chapter we focused on build configurations:
- `Debug`
- `Release`
- custom flavors such as `SimDebug`
Now we move to another core part of real project work:
**testing**.
This chapter introduces **CTest**, which is CMake’s native test runner integration.
A very important point up front:
- **CMake** configures the project
- **CTest** runs tests registered by that project
So CTest is not a separate unrelated tool.
It is part of the normal CMake testing workflow.

 ---
## Why this matters
A build that compiles is not the same as a build that is correct.
Even in small projects, tests help us:
- catch regressions
- validate expected behavior
- compare configurations
- support refactoring
- make CI more trustworthy
For a tutorial repo, tests also help demonstrate a very useful pattern:
we can build small executables whose only purpose is to verify behavior.
CTest gives us a standard way to register and run those tests.

 ---
## What we will cover
In this chapter we will cover:
- what CTest is
- how to enable testing in a CMake project
- how to add test executables
- how to register tests with `add_test()`
- how to run all tests
- how to run selected tests
- how to think about pass/fail behavior
We will keep the first example intentionally simple.
The goal is to understand the model before we bring in frameworks like GoogleTest in the next chapter.

 ---
## The key idea
CTest does not magically inspect your code.
Instead, the project defines test commands.
A very common pattern is:
1. build a small executable
2. run it through CTest
3. treat exit code `0` as pass
4. treat non-zero exit code as failure
That is the simplest and most important testing model to understand.

 ---
## What CTest is
CTest is the command-line test driver that works with CMake-generated test metadata.
In practice, the flow is:
- CMake configures the project
- the project registers tests
- CTest runs those tests from the build tree
This means tests are part of the build system, not just ad hoc scripts floating around the repo.

 ---
## What we will build
We will build:
- one small library target: `core_math`
- one test executable: `core_math_tests`
The test executable will:
- call a few functions
- verify expected results
- return `0` on success
- return non-zero on failure
Then we will register it with CTest.
This gives us the cleanest first testing example.

 ---
## Folder layout
Example folder:

 ``` text
examples/
└─ 06-testing-with-ctest/
   ├─ CMakeLists.txt
   ├─ src/
   │  ├─ math_utils.cpp
   │  └─ math_utils.h
   └─ tests/
      └─ test_math.cpp

 ```
This structure is already quite realistic:
* production code under `src/`
* test code under `tests/`

 ---
## The first important command: `enable_testing()`
To use CTest in a project, we need to enable testing.
That is done with:

 ``` cmake
enable_testing()

 ``` 
This tells CMake to generate the testing metadata needed by CTest.
Without this, `add_test()` entries will not behave as part of a normal CTest workflow.

 ---
## Registering a test
The key registration command is:

 ``` cmake
add_test(NAME some_test COMMAND some_test_executable)

 ``` 
This means:
* create a test entry named `some_test`
* when CTest runs it, execute `some_test_executable`
That is the core model.
The name is the test name CTest knows.
The command is what actually runs.

 ---
## A simple first testing design
Our test executable will do something like this:
* verify `add(2, 3) == 5`
* verify `multiply(4, 5) == 20`
* print helpful output if something fails
* return failure if any check is wrong
This is enough to demonstrate:
* build a test target
* register it
* run it through CTest
No testing framework is needed yet.
That is exactly why this chapter comes before GoogleTest.

 ---
## Why start without GoogleTest
Because the underlying testing model matters more than the framework at first.
We want readers to understand:
* a test is something CTest runs
* pass/fail is determined by the command result
* CMake knows about tests because we registered them
Once that foundation is clear, GoogleTest becomes much easier to understand.
So this chapter is about the base mechanism.

 ---
## Typical CMake shape for this chapter
A simple version of the CMake structure will look like this:

 ``` cmake
cmake_minimum_required(VERSION 3.23)
project(CTestDemo LANGUAGES CXX)
enable_testing()
add_library(core_math src/math_utils.cpp)
target_include_directories(core_math PUBLIC src)
add_executable(core_math_tests tests/test_math.cpp)
target_link_libraries(core_math_tests PRIVATE core_math)
add_test(NAME core_math_tests COMMAND core_math_tests)

 ``` 
That gives us:
* one library under test
* one test executable
* one registered CTest entry
Small, clear, and useful.

 ---
## What the test executable should do
The test executable is just a normal program.
For example, it might do this in plain English:
1. call `add(2, 3)`
2. if the result is not `5`, print an error and return `1`
3. call `multiply(4, 5)`
4. if the result is not `20`, print an error and return `1`
5. otherwise return `0`
This is enough for CTest.
CTest does not require a special testing framework to start being useful.

 ---
## Configure, build, and test flow
The normal flow becomes:

 ### Configure

 ``` bash
cmake -S examples/06-testing-with-ctest -B build/06-testing-with-ctest

 ``` 

 ### Build

 ``` bash
cmake --build build/06-testing-with-ctest

 ``` 

 ### Run tests

 ``` bash
ctest --test-dir build/06-testing-with-ctest

 ``` 
That third command is the new piece in this chapter.

 ---
## Why `ctest --test-dir ...` is nice
Using:

 ``` bash
ctest --test-dir build/06-testing-with-ctest

 ``` 
makes it very explicit which build tree’s tests are being run.
That is especially helpful when:
* you have multiple build folders
* you are switching generators
* you are comparing configurations
* you want CI logs to stay clear
It matches the out-of-source build discipline we have used throughout the tutorial.

 ---
## Running tests with more output
A very useful form is:

 ``` bash
ctest --test-dir build/06-testing-with-ctest --output-on-failure

 ``` 
This means:
* run the tests
* if a test fails, show its output
That is often the first form people use in real life, because silent failures are not very helpful.

 ---
## Running a specific test
CTest can also run selected tests by name.
For example:

 ``` bash
ctest --test-dir build/06-testing-with-ctest -R core_math_tests

 ``` 
This is useful once projects have multiple tests.
Even though our first example has only one test, it is good to introduce this early.

 ---
## What pass/fail means
For our plain executable-based test:
* return code `0` means pass
* non-zero return code means fail
That is the base convention.
This is one reason small self-checking executables are such a natural starting point for CTest.

 ---
## Why this chapter fits after configurations
Testing often interacts with configurations.
For example:
* you may want to run tests in `Debug`
* you may want to compare `Debug` and `SimDebug`
* some tests may only be meaningful in certain configurations
So it makes sense that testing comes right after the chapter on configurations.
The reader now already understands:
* build trees
* generators
* configuration selection
That makes the testing workflow much easier to explain.

 ---
## A useful mental model
Think of CTest as a registry plus a runner.
* `add_test()` registers the test
* `ctest` runs the registered tests
That is the heart of it.
You do not need to overcomplicate the first example.

 ---
## Good habits in this chapter
There are a few habits worth starting now:

 ### 1. Keep test code separate from production code
Use a `tests/` folder.

 ### 2. Test behavior, not implementation details
Check results, not internal structure.

 ### 3. Make failure messages readable
If a test fails, print enough information to see why.

 ### 4. Keep the first tests small
Small tests are easier to trust and debug.

 ### 5. Use `--output-on-failure`
It makes troubleshooting much easier.

 ---
## Common mistakes

 ### 1. Forgetting `enable_testing()`
Then tests are not properly integrated into the CTest workflow.

 ### 2. Building the test executable but never registering it
A compiled test is not automatically a CTest test.

 ### 3. Registering the wrong command
CTest runs the command you specify, so it must match the built executable correctly.

 ### 4. Assuming CTest provides assertions by itself
CTest is a runner, not a test framework.

 ### 5. Mixing test logic into the main application executable
That makes the project harder to structure and scale.

 ---
## What this chapter intentionally keeps simple
We are intentionally not doing these yet:
* GoogleTest
* parameterized tests
* automatic test discovery
* labels
* fixtures
* expected-failure tests
* coverage
* test dashboards
* advanced pass/fail regex properties
Those all exist and can be useful.
But the first goal is to understand the basic model clearly.

 ---
## This is enough for now
At this stage, the key ideas to keep are:
* CTest is CMake’s test runner integration
* `enable_testing()` turns testing on for the project
* `add_test()` registers a test command
* a plain executable returning `0`/non-zero is enough to start
* `ctest --test-dir ...` runs the tests from a build tree
That is the whole mental model for the first testing chapter.

 ---
## Takeaways
We are now adding a new capability to our projects:
not just building code, but validating it.
We learned that:
* tests can be normal executables
* CMake registers them
* CTest runs them
* pass/fail is based on the command result
This is the clean foundation we need before introducing GoogleTest.

 ---
## Next step in this chapter
The code for this chapter should define:
* a small library under test
* a small test executable
* test checks with readable failure messages
* CMake rules using `enable_testing()` and `add_test()`
That is the code we should write next.

 ---
## Next chapter
In the next chapter we will build on this foundation and bring in a real testing framework.
That is where we will cover:
* GoogleTest
* fetching it from the network
* integrating it cleanly with CMake
* and comparing framework-based tests with the plain CTest model

 ``` 

 ``` 
