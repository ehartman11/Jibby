#include "json_parser.h"

using namespace std; // Safe in implementation file only

namespace jibby {

JsonParser::JsonParser(const string& jsonText)
    : tokenizer(jsonText) {
    advance();
}

void JsonParser::advance() {
    current = tokenizer.getNextToken();
}

bool JsonParser::match(TokenType expected) {
    if (current.type == expected) {
        advance();
        return true;
    }
    return false;
}

void JsonParser::expect(TokenType expected, const string& errorMsg) {
    if (!match(expected)) {
        throw JsonParseException(errorMsg, current.line, current.column);
    }
}

Json JsonParser::parse() {
    return parseValue();
}

Json JsonParser::parseValue() {
    switch (current.type) {
        case TokenType::LEFT_BRACE:   return parseObject();
        case TokenType::LEFT_BRACKET: return parseArray();
        case TokenType::STRING:       return parseString();
        case TokenType::NUMBER:       return parseNumber();
        case TokenType::TRUE:         
        case TokenType::FALSE:        
        case TokenType::NUL:          return parseLiteral();
        default:
            throw JsonParseException("Unexpected token", current.line, current.column);
    }
}

Json JsonParser::parseObject() {
    Json obj = Json::object();
    advance(); // consume '{'

    if (match(TokenType::RIGHT_BRACE)) return obj;

    do {
        if (current.type != TokenType::STRING)
            throw JsonParseException("Expected string key in object", current.line, current.column);

        std::string key = current.value;
        advance(); // consume key token

        expect(TokenType::COLON, "Expected ':' after key");

        obj.asObject()[key] = parseValue();
    } while (match(TokenType::COMMA));

    expect(TokenType::RIGHT_BRACE, "Expected '}' at end of object");
    return obj;
}

Json JsonParser::parseArray() {
    Json arr = Json::array();
    advance(); // consume '['

    if (match(TokenType::RIGHT_BRACKET)) return arr;

    do {
        arr.asArray().push_back(parseValue());
    } while (match(TokenType::COMMA));

    expect(TokenType::RIGHT_BRACKET, "Expected ']' at end of array");
    return arr;
}

Json JsonParser::parseString() {
    string val = current.value;
    advance();
    return Json(val);
}

Json JsonParser::parseNumber() {
    double num = stod(current.value);
    advance();
    return Json(num);
}

Json JsonParser::parseLiteral() {
    if (current.type == TokenType::TRUE) {
        advance();
        return Json(true);
    }
    if (current.type == TokenType::FALSE) {
        advance();
        return Json(false);
    }
    if (current.type == TokenType::NUL) {
        advance();    
        return Json(nullptr);
    }
    throw JsonParseException("Unexpected literal", current.line, current.column);
}

} // namespace jibby
