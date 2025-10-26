#ifndef JIBBY_JSON_H
#define JIBBY_JSON_H

#include <initializer_list>
#include "json_exception.h"
#include "json_types.h"

namespace jibby {

    // Forward declare the JsonIterator class for compiler processing
    class JsonIterator;

    // Json class
    class Json {
        // Enum of Json types. This will allow for proper declaration, identification, navigation, and manipulation later 
        enum class Type {
            Null,
            Boolean,
            Number,
            String,
            Object,
            Array
        };

        private:
            Type type; // Json type from Type enum
            variant<std::nullptr_t, bool, double, string, Object, Array> value; // value being held: can be one of any of the declared types in variant<...>

        public:
            // Constructors: will instantiate the Json object with the proper type
            Json();                       
            Json(std::nullptr_t);         
            Json(bool b);                  
            Json(double num);              
            Json(const string& str);       
            Json(const char* str);         
            Json(const Object& obj);       
            Json(const Array& arr);        

            // Type Checks: verifies type of Json object and returns the boolean of the check against the given type
            bool isNull() const    { return type == Type::Null; }
            bool isBoolean() const { return type == Type::Boolean; }
            bool isNumber() const  { return type == Type::Number; }
            bool isString() const  { return type == Type::String; }
            bool isObject() const  { return type == Type::Object; }
            bool isArray() const   { return type == Type::Array; }

            // Access constants
            const Object& asObject() const;
            const Array& asArray() const;
            const string& asString() const;
            double asNumber() const;
            bool asBoolean() const;           

            // Access mutables
            Object& asObject();
            Array& asArray();
            string& asString();
            double& asNumber();
            bool& asBoolean();

            // Iterators for mapped objects and arrays using [] 
            Json& operator[](const string& key);
            Json& operator[](size_t index);
            const Json& operator[](const string& key) const;
            const Json& operator[](size_t index) const;


            // Assignments 
            Json& operator=(const string& str);     
            Json& operator=(const char* str);      
            Json& operator=(bool b);                
            Json& operator=(std::nullptr_t);


            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>>>
            Json& operator=(T num) {
                type = Type::Number;
                value = static_cast<double>(num);
                return *this;
            }          

            // Get the Json type of the object
            Type getType() const { return type; }

            // Object Iteration: get the beginning and end of the map/array
            JsonIterator begin();
            JsonIterator end();
            JsonIterator begin() const;
            JsonIterator end() const;

            // Factory Helpers
            static Json object();
            static Json array();

            // Serialize the data
            string serialize(int indent = 0, int depth = 0) const;

            // File I/O for .json files 
            static Json load(const string& filepath);
            void save(const string& filepath, bool pretty = false) const;

        };

}

// 
#include "json_iterator.h"
#endif
