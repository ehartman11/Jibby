#ifndef JIBBY_JSON_TOKENIZER_H
#define JIBBY_JSON_TOKENIZER_H

#include "json.h"
#include "json_token.h"
#include "json_exception.h"

namespace jibby {

    class JsonTokenizer {
        private:
            const string input;
            size_t pos = 0;
            size_t line = 1;
            size_t column = 1;

        public:
            explicit JsonTokenizer(const string& jsonText) : input(jsonText) {}
            Token getNextToken();

        private:
            char peek() const;
            char advance();
            void skipWhitespace();
            Token stringToken();
            Token numberToken(char c);
            Token literalToken(char c);
            bool isAtEnd() const;
    };

}

#endif 
