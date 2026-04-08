#include "json_tokenizer.h"
#include <cctype>      
#include <sstream>     
#include <stdexcept>  

using namespace std;   

namespace jibby {

namespace {

int hexValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

void appendUtf8(string& out, unsigned codePoint) {
    if (codePoint <= 0x7F) {
        out.push_back(static_cast<char>(codePoint));
    } else if (codePoint <= 0x7FF) {
        out.push_back(static_cast<char>(0xC0 | (codePoint >> 6)));
        out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xE0 | (codePoint >> 12)));
        out.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }
}

} // namespace

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
                case 'u': {
                    unsigned codePoint = 0;
                    for (int i = 0; i < 4; ++i) {
                        if (isAtEnd()) {
                            throw JsonParseException("Unterminated unicode escape", line, column);
                        }

                        char hex = advance();
                        int value = hexValue(hex);
                        if (value < 0) {
                            throw JsonParseException("Invalid unicode escape", line, column - 1);
                        }
                        codePoint = (codePoint << 4) | static_cast<unsigned>(value);
                    }
                    appendUtf8(result, codePoint);
                    break;
                }
                default:
                    throw JsonParseException("Invalid escape character: " + string(1, esc), line, column - 1);
            }
        } else {
            if (static_cast<unsigned char>(c) < 0x20) {
                throw JsonParseException("Unescaped control character in string", line, column - 1);
            }
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

    if (firstChar == '-') {
        if (isAtEnd() || !isdigit(static_cast<unsigned char>(peek()))) {
            throw JsonParseException("Invalid number", startLine, startColumn);
        }
        numStr.push_back(advance());
    }

    if (numStr.back() == '0') {
        if (!isAtEnd() && isdigit(static_cast<unsigned char>(peek()))) {
            throw JsonParseException("Leading zeroes are not allowed", line, column);
        }
    } else {
        while (!isAtEnd() && isdigit(static_cast<unsigned char>(peek()))) {
            numStr.push_back(advance());
        }
    }

    if (!isAtEnd() && peek() == '.') {
        numStr.push_back(advance());
        if (isAtEnd() || !isdigit(static_cast<unsigned char>(peek()))) {
            throw JsonParseException("Invalid number", line, column);
        }
        while (!isAtEnd() && isdigit(static_cast<unsigned char>(peek()))) {
            numStr.push_back(advance());
        }
    }

    if (!isAtEnd() && (peek() == 'e' || peek() == 'E')) {
        numStr.push_back(advance());
        if (!isAtEnd() && (peek() == '+' || peek() == '-')) {
            numStr.push_back(advance());
        }
        if (isAtEnd() || !isdigit(static_cast<unsigned char>(peek()))) {
            throw JsonParseException("Invalid exponent", line, column);
        }
        while (!isAtEnd() && isdigit(static_cast<unsigned char>(peek()))) {
            numStr.push_back(advance());
        }
    }

    if (!isAtEnd()) {
        char c = peek();
        if (isalpha(static_cast<unsigned char>(c)) || c == '.' || c == '+' || c == '-') {
            throw JsonParseException("Invalid number", line, column);
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
