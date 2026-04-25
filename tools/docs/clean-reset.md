# Clean Reset Workflow
When a CMake build gets into a strange state, use one of these reset paths.
---
## Option A - Manual clean reset
This works on all tutorial-supported CMake versions.
### Windows cmd
```bat
rmdir /s /q build
cmake -S . -B build
cmake --build build