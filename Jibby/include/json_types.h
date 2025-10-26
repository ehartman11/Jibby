#ifndef JIBBY_JSON_TYPES_H
#define JIBBY_JSON_TYPES_H

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

namespace jibby {
    // Forward declare Json class for compiler processing 
    class Json;

    // Type aliases to make typing cleaner throughout codebase
    using string = std::string;
 
    template<typename K, typename V>
    using hashmap = std::unordered_map<K, V>;

    template<typename T>
    using vector = std::vector<T>;

    template<typename... Ts>
    using variant = std::variant<Ts...>;

    using Object = hashmap<string, Json>;
    using Array = vector<Json>;
}

#endif