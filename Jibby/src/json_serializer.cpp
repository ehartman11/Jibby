#include "json_serializer.h"

using namespace std; 

namespace jibby {

string JsonSerializer::serialize(const Json& value, int indent) {
    return value.serialize(indent, 0);
}

} 
