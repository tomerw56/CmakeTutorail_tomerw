# Chapter 03 - Building and Loading a DLL
In the previous chapters we created a small executable and explored generator behavior.
Now we move to a more interesting and more realistic Windows-oriented example:
we will build a DLL, export functions from it, and load it at runtime from a separate executable.
This is a very useful step because it shows that libraries are not only about static linking.
Sometimes an application loads functionality dynamically:
- plugins
- optional modules
- simulation backends
- tools loaded on demand
- versioned components shipped separately
This chapter introduces that pattern in a small and focused way.
---
## Why this matters
A plain library-linking example is useful, but dynamic loading teaches more:
- how to build a shared library
- how symbol exports work on Windows
- how an executable can load a DLL at runtime
- how to locate functions with `GetProcAddress`
- how to handle runtime failure when a DLL or export is missing
This is much closer to the kinds of things many real Windows codebases do.
It also reinforces an important CMake idea:
we are still defining targets and relationships, but not every relationship is a normal link-time dependency.
---
## What we will build
We will build two targets:
- `math_dll` — a DLL that exports a small C-style API
- `dll_loader` — an executable that loads the DLL dynamically at runtime
The DLL will export two functions:
- `add`
- `multiply`
The executable will:
1. load the DLL
2. get the function addresses
3. call the functions
4. print the results
---
## Why we use a DLL here
A DLL is a dynamic library on Windows.
Using a DLL makes it possible to:
- replace the implementation without rebuilding the executable
- load a module only when needed
- fail gracefully if a component is missing
- separate delivery of app and module
That is different from a static library, where the code becomes part of the final executable at link time.
---
## Why we use runtime loading instead of normal linking
In this chapter, the executable will **not** call the DLL through normal link-time usage.
Instead, it will:
- call `LoadLibrary`
- call `GetProcAddress`
- use function pointers
That is intentional.
This lets us demonstrate:
- explicit runtime loading
- exported symbol names
- failure handling
- the difference between "a DLL exists" and "the program can successfully use it"
This makes the chapter more concrete and more interesting than a plain shared-library link demo.
---
## Why we export C-style functions
We will export the DLL API with:
```cpp
extern "C"
````
This is useful because it avoids C++ name mangling.
That makes runtime lookup simpler and clearer:
* exported name: `add`
* exported name: `multiply`
instead of compiler-specific mangled C++ names.
For a teaching example, this is the cleanest choice.
---
## Folder layout
Example folder:
```text
examples/
└─ 03-libraries/
   ├─ CMakeLists.txt
   ├─ app/
   │  └─ main.cpp
   └─ math_dll/
      ├─ math_exports.h
      ├─ math_api.h
      └─ math_api.cpp
```
This gives us:
* a DLL implementation area
* a loader application area
* one CMake file at the top for now
Later chapters may split larger examples across multiple `CMakeLists.txt` files, but for this chapter one top-level file is enough.
---
## High-level build design
At the CMake level, the project will look like this:
* define a shared library target:
  * `math_dll`
* define an executable target:
  * `dll_loader`
* compile the DLL with an export macro enabled
* copy the built DLL next to the executable after build
That last step matters because on Windows the easiest way to run the demo is to have the DLL beside the EXE.
---
## The DLL side
The DLL will contain:
* a header for export/import control
* a header for the API
* a source file with the implementation
### Export/import control header
We will use a macro like this:
* while building the DLL:
  * export symbols
* when consuming the DLL normally:
  * import symbols
In this chapter, the loader uses runtime lookup, so it does not strictly need `dllimport` to call the functions.
Still, showing the export/import pattern is good practice and worth learning early.
---
## The executable side
The executable will:
* include the Windows API
* define function pointer types
* load the DLL by file name
* look up exported functions
* validate that each lookup succeeded
* call the functions
* release the DLL
This is a very common pattern in plugin-style systems and optional-component loading.
---
## Expected runtime flow
When the program runs, the flow is:
1. `dll_loader.exe` starts
2. it tries to load `math_dll.dll`
3. if loading fails, it prints an error and exits
4. if loading succeeds, it looks up `add`
5. it looks up `multiply`
6. if either lookup fails, it prints an error and exits
7. if both succeed, it calls the functions and prints results
8. it unloads the DLL
This is the behavior we want to demonstrate.
---
## Why this is a good CMake chapter
Even though the runtime loading is a Windows API topic, this is still very much a CMake chapter because it teaches:
* `add_library(... SHARED ...)`
* target-specific compile definitions
* organizing code into separate targets
* post-build copy steps
* output layout awareness
In other words, we are not leaving CMake territory.
We are just applying it to a more realistic example.
---
## The key CMake ideas in this chapter
We will use these ideas:
### 1. `add_library(... SHARED ...)`
This creates a shared library target.
On Windows, that means building a DLL.
### 2. `target_compile_definitions()`
We will define something like `BUILDING_MATH_DLL` for the DLL target so the export macro knows when to use `__declspec(dllexport)`.
### 3. `add_executable()`
This creates the loader application target.
### 4. post-build copy
We will copy the built DLL next to the EXE after build.
That makes the example easy to run.
---
## Why we need an export macro
Windows DLL exports are not automatic in the simplest style we want to teach here.
We need to mark exported functions so that they become visible outside the DLL.
That is why we use a macro pattern such as:
* `__declspec(dllexport)` when building the DLL
* `__declspec(dllimport)` when consuming it normally
This pattern is very common in Windows C++ codebases.
In this tutorial, the runtime-loaded EXE does not require normal import linkage to call the functions, but the export side is still essential.
---
## Why the loader EXE still matters as a separate target
The second executable is not just there to "run something."
It shows a different kind of dependency:
* the EXE depends on the DLL being present at runtime
* but not through normal direct symbol resolution at link time
That distinction is important.
There are really two relationships here:
* a build relationship: both targets are produced by the same project
* a runtime relationship: one target loads the other dynamically
That is a useful concept to expose early.
---
## Running the example
The chapter will eventually show commands like:
### Configure
```bash
cmake -S examples/03-libraries -B build/03-libraries
```
### Build
```bash
cmake --build build/03-libraries
```
### Run on Windows
```powershell
.\build\03-libraries\dll_loader.exe
```
or, with a multi-config generator such as Visual Studio:
```powershell
.\build\03-libraries\Debug\dll_loader.exe
```
The exact output path depends on the generator and configuration, just as we discussed in Chapter 02.
---
## Expected output
When everything works, the loader should print something like:
```text
2 + 3 = 5
4 * 5 = 20
```
If the DLL is missing or a function export cannot be found, the loader should print a failure message and exit with a non-zero code.
That is actually part of the lesson:
dynamic loading means runtime failure paths must be handled explicitly.
---
## What we intentionally keep simple
This chapter is focused.
So we are intentionally not adding all of these yet:
* version resources for the DLL
* import libraries as the main consumption model
* plugin search paths
* wide-character `LoadLibraryW`
* Unicode path handling
* `SetDllDirectory`
* delay loading
* cross-platform `dlopen` / `dlsym`
* subdirectory-based CMake organization
Those are all valid topics.
They are just not needed for the first clean DLL-loading example.
---
## Why this is a Windows-focused chapter
This example intentionally uses:
* `LoadLibrary`
* `GetProcAddress`
* `FreeLibrary`
Those are Windows APIs.
That is fine here because the tutorial is already very Windows-aware, and DLL behavior is a natural topic for this audience.
If we later want a portable comparison, we can add a note or separate example showing:
* `dlopen`
* `dlsym`
* `dlclose`
for Linux-like environments.
But for now, Windows is the point.
---
## Common mistakes we want this chapter to prevent
### 1. Thinking a DLL is the same as a static library
It is not.
It is loaded dynamically and has different runtime behavior.
### 2. Forgetting to export functions
If a function is not exported, `GetProcAddress` will fail.
### 3. Exporting C++ functions and expecting simple names
Name mangling makes lookup harder.
That is why this tutorial uses `extern "C"`.
### 4. Assuming the DLL will always be found automatically
The runtime loader has to find the DLL.
Copying it next to the executable is the easiest demo-friendly setup.
### 5. Forgetting runtime error handling
Dynamic loading can fail even if compilation succeeded.
---
## This is enough for now
At this stage, the key ideas to keep are:
* we can build a DLL with CMake using a shared library target
* exported functions need to be visible from outside the DLL
* a separate EXE can load the DLL dynamically at runtime
* runtime loading is different from normal linking
* CMake still helps organize all of this through targets and post-build steps
That is enough mental model for the code.
---
## Takeaways
This chapter introduces a more realistic library scenario than plain static linking.
We are moving from:
* "build one executable"
to:
* "build a DLL"
* "build a loader EXE"
* "connect them through runtime behavior"
That gives us a stronger example for later chapters and a better foundation for Windows-specific topics such as version information on DLLs.
---
## Next step in this chapter
The code for this chapter will define:
* a DLL export macro
* a small exported math API
* a DLL implementation
* a loader executable that uses `LoadLibrary` and `GetProcAddress`
* CMake rules to build both targets and place the DLL where the EXE can find it
That is the code we will write next.
---
## Next chapter
In the next chapter, we will step back from the DLL mechanics and focus on one of the most important CMake concepts:
* `PRIVATE`
* `PUBLIC`
* `INTERFACE`
That is where usage requirements start to become explicit and powerful.
```
