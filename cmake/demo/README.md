# ?

A starter code for using clangd/lldb and cmake on mac/linux

Avoid the hassle of searching every time

# Use RUST if NOT necessary

# Project struct
- src/module1 is a static library with no dependencies
- src/module2 is a dynamic library with no dependencies
- src/module3 is a static library that depends on module1 and module2
- src/module4 is a dynamic library that depends on module3 and module2

- third_party
    - googletest is used for unit testing

# CLI

```
cmake -B build -S . -G Ninja
cmake --build build -j
```