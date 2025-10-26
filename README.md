Fast as a jiffy, clean as JSON.

Jibby is a high-performance, ergonomic JSON library for C++.
It started as a personal project — and a fun mispronunciation of "JSON library" by my son — and grew into a lightweight, developer-friendly tool for parsing and generating JSON.

Why Jibby?

- Fast as a jiffy – no wasted allocations, optimized parsing.

- Clean as JSON – simple, ergonomic API.

- Lightweight – zero external dependencies.

- Personal and community-driven


example usage: 

#include <jibby/json.h>
#include <iostream>

int main() {
    Json data = Json::load(R"(path\config.json)");
    std::cout << data.serialize(4) << std::endl; // (4) used for pretty printing
    
    for (auto& [key, val] : data) {
        std::cout << key << ": "<< val.serialize() << std::endl;
    }

    data["complete"] = true;
    data.save(R"(path\config_out.json)", true);
}
