#include "json.h"
#include <iostream>
#include <variant>
using namespace jibby;

int main() {
    Json data = Json::load(R"(C:\Users\ephar\Jibby\tests\config.json)");
    std::cout << "Serialized immediately after load:/n" << data.serialize(4) << std::endl;
    
    for (auto& [key, val] : data) {
        std::cout << key << ": "<< val.serialize() << std::endl;
    }

    data["complete"] = true;
    data.save(R"(C:\Users\ephar\Jibby\tests\config_out.json)", true);
    
}