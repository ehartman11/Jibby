#ifndef JIBBY_JSON_TOKEN_H
#define JIBBY_JSON_TOKEN_H

#include "json.h"

namespace jibby {

    // Token for identification during parsing of json strings and objects
    enum class TokenType {
        LEFT_BRACE,    
        RIGHT_BRACE,   
        LEFT_BRACKET,  
        RIGHT_BRACKET, 
        COLON,         
        COMMA,         
        STRING,
        NUMBER,
        TRUE,
        FALSE,
        NUL,
        END_OF_FILE
    };

    // Token attributes with default values
    struct Token {
        TokenType type = TokenType::END_OF_FILE;
        string value = ""; 
        size_t line = 1;
        size_t column = 1;

        // Constructors
        Token() = default;

        Token(TokenType t, const string& v = "", size_t l = 1, size_t c = 1)
            : type(t), value(v), line(l), column(c) {}
    };

}

#endif
