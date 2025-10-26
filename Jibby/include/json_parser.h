#ifndef JIBBY_JSON_PARSER_H
#define JIBBY_JSON_PARSER_H

#include "json.h"
#include "json_tokenizer.h"
#include "json_exception.h"

namespace jibby {

    // class for parsing string of Json object
    class JsonParser {

        public:
            explicit JsonParser(const string& jsonText);
            Json parse();

        private:
            JsonTokenizer tokenizer;
            Token current;

            void advance();
            bool match(TokenType expected);
            void expect(TokenType expected, const string& errorMsg);

            Json parseValue();
            Json parseObject();
            Json parseArray();
            Json parseString();
            Json parseNumber();
            Json parseLiteral();
    };

} 

#endif
