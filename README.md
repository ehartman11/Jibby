# Jibby

Fast as a jiffy, clean as JSON.

Jibby is a lightweight JSON library for C++ built around a simple idea: parsing and writing JSON should feel straightforward.

It started as a personal project, sparked by a fun mispronunciation of "JSON library" by my son, and slowly grew into something worth shaping into a real tool. That personal beginning is still part of the project. Jibby is meant to be small, readable, approachable, and useful.

## Why Jibby?

- Simple API for loading, inspecting, modifying, and saving JSON
- Lightweight with zero external dependencies
- Built from scratch in C++
- Easy to read, extend, and learn from

## Example

```cpp
#include "json.h"
#include <iostream>

using jibby::Json;

int main() {
    Json data = Json::load(R"(path\config.json)");
    std::cout << data.serialize(4) << std::endl;

    for (auto& [key, val] : data) {
        std::cout << key << ": " << val.serialize() << std::endl;
    }

    data["complete"] = true;
    data.save(R"(path\config_out.json)", true);
}
```

## What It Does

Jibby currently supports:

- Parsing JSON from strings and files
- Serializing JSON values back to text
- Working with objects, arrays, strings, numbers, booleans, and null
- Iterating through objects and arrays
- Pretty-printing output

## Project Status

Jibby is still a small personal library, but it is actively being improved. The current workspace includes regression tests around parsing, serialization, escaping, Unicode handling, and invalid input cases.

This project is best treated as a lightweight utility library and a from-scratch implementation project, not a replacement for larger established JSON libraries.

## Why Publish It?

Because not every useful project has to reinvent a field.

Jibby is not trying to be a novel new format or a breakthrough parser. Its value is in being compact, understandable, dependency-free, and personal. If it helps someone learn, prototype, or solve a small problem without pulling in a large dependency, then it has done something worthwhile.

## Building

Jibby now includes a simple `CMakeLists.txt` so it can be built more easily across environments.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

This builds the `jibby` library and the `jibby_tests` executable.

## Notes

If you use Jibby, expect a project that is still growing. That means a cleaner codebase to explore, but also a library that is still earning its shape over time.
