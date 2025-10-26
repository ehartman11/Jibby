#include "json_tokenizer.h"
#include <cctype>      
#include <sstream>     
#include <stdexcept>  

using namespace std;   

namespace jibby {

// Utility Methods 

bool JsonTokenizer::isAtEnd() const {
    return pos >= input.size();
}

char JsonTokenizer::peek() const {
    if (isAtEnd()) return '\0';
    return input[pos];
}

char JsonTokenizer::advance() {
    if (isAtEnd()) return '\0';

    char c = input[pos++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

void JsonTokenizer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (isspace(static_cast<unsigned char>(c))) {
            advance();
        } else {
            break;
        }
    }
}

// Main Tokenizer Method 
Token JsonTokenizer::getNextToken() {
    skipWhitespace();

    if (isAtEnd()) {
        return Token(TokenType::END_OF_FILE, "", line, column);
    }

    char c = advance();

    // Single-character tokens
    switch (c) {
        case '{': return Token(TokenType::LEFT_BRACE, "{", line, column - 1);
        case '}': return Token(TokenType::RIGHT_BRACE, "}", line, column - 1);
        case '[': return Token(TokenType::LEFT_BRACKET, "[", line, column - 1);
        case ']': return Token(TokenType::RIGHT_BRACKET, "]", line, column - 1);
        case ':': return Token(TokenType::COLON, ":", line, column - 1);
        case ',': return Token(TokenType::COMMA, ",", line, column - 1);
        case '"': return stringToken();
    }

    // Numbers 
    if (isdigit(static_cast<unsigned char>(c)) || c == '-') {
        return numberToken(c);
    }

    // Literals (true, false, null)
    if (isalpha(static_cast<unsigned char>(c))) {
        return literalToken(c);
    }

    // Unexpected character 
    throw JsonParseException("Unexpected character: " + string(1, c), line, column - 1);
}

// String Tokens
Token JsonTokenizer::stringToken() {
    size_t startLine = line;
    size_t startColumn = column;

    string result;

    while (!isAtEnd()) {
        char c = advance();
        if (c == '"') {
            // End of string
            return Token(TokenType::STRING, result, startLine, startColumn);
        }

        // Handle escape sequences
        if (c == '\\') {
            if (isAtEnd()) throw JsonParseException("Unterminated string escape sequence", line, column);

            char esc = advance();
            switch (esc) {
                case '"':  result.push_back('"');  break;
                case '\\': result.push_back('\\'); break;
                case '/':  result.push_back('/');  break;
                case 'b':  result.push_back('\b'); break;
                case 'f':  result.push_back('\f'); break;
                case 'n':  result.push_back('\n'); break;
                case 'r':  result.push_back('\r'); break;
                case 't':  result.push_back('\t'); break;
                default:
                    throw JsonParseException("Invalid escape character: " + string(1, esc), line, column - 1);
            }
        } else {
            result.push_back(c);
        }
    }

    throw JsonParseException("Unterminated string literal", startLine, startColumn);
}

// Number Tokens 
Token JsonTokenizer::numberToken(char firstChar) {
    size_t startLine = line;
    size_t startColumn = column - 1;

    string numStr;
    numStr.push_back(firstChar);

    while (!isAtEnd()) {
        char c = peek();
        if (isdigit(static_cast<unsigned char>(c)) || c == '.' || c == 'e' || c == 'E' || c == '+' || c == '-') {
            numStr.push_back(advance());
        } else {
            break;
        }
    }

    return Token(TokenType::NUMBER, numStr, startLine, startColumn);
}

// Literal Tokens (true, false, null) 
Token JsonTokenizer::literalToken(char firstChar) {
    size_t startLine = line;
    size_t startColumn = column - 1;

    string literal(1, firstChar);

    // Collect full literal
    while (!isAtEnd() && isalpha(static_cast<unsigned char>(peek()))) {
        literal.push_back(advance());
    }

    if (literal == "true")  return Token(TokenType::TRUE, literal, startLine, startColumn);
    if (literal == "false") return Token(TokenType::FALSE, literal, startLine, startColumn);
    if (literal == "null")  return Token(TokenType::NUL, literal, startLine, startColumn);

    throw JsonParseException("Unknown literal: " + literal, startLine, startColumn);
}

} // namespace jibby
