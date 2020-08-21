# Obelisk

'obelisk' is a simple library of C++ utilities. Made for fun, learning and future reference.

## Build

Build is defined using CMake. Currently building on Clang 11.0.0 using C++17.

CMake usage based on http://unclejimbo.github.io/2018/06/08/Modern-CMake-for-Library-Developers/

## Test

Tests use the single header include of [Catch2](https://github.com/catchorg/Catch2) v2.12.3

## Naming/style conventions

- Follows the included clang-format style
- lowerCamelCase for variable names and functions
- UpperCamelCase for class names
- snake_case for namespace
- ALL_CAPS for constants
- UpperCamelCase for C++ code filenames
- .hpp extension for header only files
- .h extension for header files with associated .cpp

## License

WTFPL
