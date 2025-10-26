#include "json_exception.h"
#include <sstream>

using namespace std;

namespace jibby {

    JsonException::JsonException(const std::string& msg)
        : runtime_error(msg) {}

    JsonParseException::JsonParseException(const std::string& msg, size_t line, size_t column)
        : JsonException(buildMessage(msg, line, column)) {}

    string JsonParseException::buildMessage(const std::string& msg, size_t line, size_t column) {
        stringstream ss;
        ss << msg << " (line " << line << ", column " << column << ")";
        return ss.str();
    }

}
