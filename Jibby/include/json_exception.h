#ifndef JIBBY_JSON_EXCEPTION_H
#define JIBBY_JSON_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace jibby {

    // Class for all JSON-related errors
    class JsonException : public std::runtime_error {
        public:
            explicit JsonException(const std::string& msg);
        };

        class JsonParseException : public JsonException {
        public:
            JsonParseException(const std::string& msg, size_t line, size_t column);

        private:
            static std::string buildMessage(const std::string& msg, size_t line, size_t column);
    };

} 

#endif
