#include "json.h"
#include "json_exception.h"
#include "json_iterator.h"
#include "json_io.h"
#include <sstream>

using namespace std; // Safe here in a .cpp file only

namespace jibby {

// ---- Constructors ----
Json::Json() : type(Type::Null), value(nullptr) {}
Json::Json(std::nullptr_t) : type(Type::Null), value(nullptr) {}
Json::Json(bool b) : type(Type::Boolean), value(b) {}
Json::Json(double num) : type(Type::Number), value(num) {}
Json::Json(const string& str) : type(Type::String), value(str) {}
Json::Json(const char* str) : type(Type::String), value(string(str)) {}
Json::Json(const Object& obj) : type(Type::Object), value(obj) {}
Json::Json(const Array& arr) : type(Type::Array), value(arr) {}

// ---- Const Accessors ----
const Object& Json::asObject() const {
    if (!isObject()) throw runtime_error("Json value is not an object");
    return get<Object>(value);
}

const Array& Json::asArray() const {
    if (!isArray()) throw runtime_error("Json value is not an array");
    return get<Array>(value);
}

const string& Json::asString() const {
    if (!isString()) throw runtime_error("Json value is not a string");
    return get<string>(value);
}

double Json::asNumber() const {
    if (!isNumber()) throw runtime_error("Json value is not a number");
    return get<double>(value);
}

bool Json::asBoolean() const {
    if (!isBoolean()) throw runtime_error("Json value is not a boolean");
    return get<bool>(value);
}

// ---- Mutable Accessors ----
Object& Json::asObject() {
    if (!isObject()) throw runtime_error("Json value is not an object");
    return get<Object>(value);
}

Array& Json::asArray() {
    if (!isArray()) throw runtime_error("Json value is not an array");
    return get<Array>(value);
}

string& Json::asString() {
    if (!isString()) throw runtime_error("Json value is not a string");
    return get<string>(value);
}

double& Json::asNumber() {
    if (!isNumber()) throw runtime_error("Json value is not a number");
    return get<double>(value);
}

bool& Json::asBoolean() {
    if (!isBoolean()) throw runtime_error("Json value is not a boolean");
    return get<bool>(value);
}

// ---- Index Operators ----
const Json& Json::operator[](const string& key) const {
    if (!isObject()) {
        throw JsonException("Cannot use operator[] on non-object JSON value");
    }

    const auto& obj = asObject();
    auto it = obj.find(key);
    if (it == obj.end()) {
        throw JsonException("Key not found: " + key);
    }
    return it->second;
}

const Json& Json::operator[](size_t index) const {
    if(!isArray()) {
        throw JsonException("Cannot use operator[] with index on non-array JSON value");
    }
    auto& arr = asArray();
    if (index >= arr.size()) {
        throw JsonException("Array index out of bounds: " + to_string(index));
    }
    return arr[index];
}

Json& Json::operator[](const string& key) {
    if (!isObject()) {
        throw JsonException("Cannot use operator[] on non-object JSON value");
    }
    return asObject()[key];
}

Json& Json::operator[](size_t index) {
    if(!isArray()) {
        throw JsonException("Cannot use operator[] with index on non-array JSON value");
    }
    auto& arr = asArray();
    if (index >= arr.size()) {
        throw JsonException("Array index out of bounds: " + to_string(index));
    }
    return arr[index];
}

// ---- Factory Helpers ----
Json Json::object() {
    return Json(Object{});
}

Json Json::array() {
    return Json(Array{});
}

// ---- Assignment Operators ----
Json& Json::operator=(const string& str) {
    type = Type::String;
    value = str;
    return *this;
}

Json& Json::operator=(const char* str) {
    type = Type::String;
    value = string(str);
    return *this;
}

Json& Json::operator=(bool b) {
    type = Type::Boolean;
    value = b;
    return *this;
}

Json& Json::operator=(std::nullptr_t) {
    type = Type::Null;
    value = nullptr;
    return *this;
}

// ---- Iteration ---- 
JsonIterator Json::begin() {
    if (isObject()) return JsonIterator(asObject().begin());
    if (isArray())  return JsonIterator(asArray().begin());
    throw JsonException("Cannot iterate over non-object/array JSON value");
}

JsonIterator Json::end() {
    if (isObject()) return JsonIterator(asObject().end());
    if (isArray())  return JsonIterator(asArray().end());
    throw JsonException("Cannot iterate over non-object/array JSON value");
}

JsonIterator Json::begin() const {
    if (isObject()) return JsonIterator(asObject().begin());
    if (isArray())  return JsonIterator(asArray().begin());
    throw JsonException("Cannot iterate over non-object/array JSON value");
}

JsonIterator Json::end() const {
    if (isObject()) return JsonIterator(asObject().end());
    if (isArray())  return JsonIterator(asArray().end());
    throw JsonException("Cannot iterate over non-object/array JSON value");
}

// ---- File I/O ----
Json Json::load(const string& filepath) {
    return JsonIO::read(filepath);
}

void Json::save(const string& filepath, bool pretty) const {
    JsonIO::write(*this, filepath, pretty);
}

// ---- Serialization ----
string Json::serialize(int indent, int depth) const {
    ostringstream oss;

    switch (type) {
        case Type::Null:
            oss << "null";
            break;
        case Type::Boolean:
            oss << (get<bool>(value) ? "true" : "false");
            break;
        case Type::Number:
            oss << get<double>(value);
            break;
        case Type::String:
            oss << "\"" << get<string>(value) << "\"";
            break;

        case Type::Object: {
            const auto& obj = get<Object>(value);
            oss << "{";
            bool first = true;
            for (const auto& [key, val] : obj) {
                if (!first) oss << ",";
                if (indent > 0) {
                    oss << "\n" << string(indent * (depth + 1), ' ');
                }
                oss << "\"" << key << "\": " << val.serialize(indent, depth + 1);
                first = false;
            }
            if (indent > 0 && !obj.empty()) {
                oss << "\n" << string(indent * depth, ' ');
            }
            oss << "}";
            break;
        }

        case Type::Array: {
            const auto& arr = get<Array>(value);
            oss << "[";
            bool first = true;
            for (const auto& val : arr) {
                if (!first) oss << ",";
                if (indent > 0) {
                    oss << "\n" << string(indent * (depth + 1), ' ');
                }
                oss << val.serialize(indent, depth + 1);
                first = false;
            }
            if (indent > 0 && !arr.empty()) {
                oss << "\n" << string(indent * depth, ' ');
            }
            oss << "]";
            break;
        }
    }

    return oss.str();
}

}
