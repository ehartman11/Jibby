#include "json.h"
#include "json_exception.h"
#include "json_parser.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

using jibby::Json;
using jibby::JsonException;
using jibby::JsonParser;

namespace {

void expectThrows(const std::function<void()>& fn, const std::string& messageFragment, const std::string& testName) {
    try {
        fn();
        assert(false && "Expected exception was not thrown");
    } catch (const JsonException& ex) {
        if (ex.what() == nullptr) {
            assert(false && "Exception message should not be null");
        }
        const std::string message = ex.what();
        if (message.find(messageFragment) == std::string::npos) {
            std::cerr << testName << " threw unexpected message: " << message << '\n';
            assert(false && "Exception message did not contain expected fragment");
        }
    }
}

void testLoadAndSaveRoundTrip() {
    Json data = Json::load("tests/config.json");
    assert(data.isObject());
    assert(data["app_name"].asString() == "json_test");
    assert(data["complete"].asBoolean() == false);

    data["complete"] = true;

    const std::filesystem::path outPath = std::filesystem::path("tests") / "config_out.json";
    data.save(outPath.string(), true);

    Json reloaded = Json::load(outPath.string());
    assert(reloaded["complete"].asBoolean() == true);
}

void testRejectsTrailingContent() {
    expectThrows([] {
        JsonParser parser("{\"ok\":true}[]");
        parser.parse();
    }, "Unexpected trailing content", "testRejectsTrailingContent");
}

void testEscapesStringsOnSerialize() {
    Json value = Json::object();
    value["quote\"slash\\newline"] = "line1\nline2\t\"quoted\"";

    const std::string serialized = value.serialize();
    assert(serialized.find("\\\"") != std::string::npos);
    assert(serialized.find("\\\\") != std::string::npos);
    assert(serialized.find("\\n") != std::string::npos);
    assert(serialized.find("\\t") != std::string::npos);

    Json reparsed = JsonParser(serialized).parse();
    assert(reparsed["quote\"slash\\newline"].asString() == "line1\nline2\t\"quoted\"");
}

void testUnicodeEscapesParse() {
    Json parsed = JsonParser("{\"letter\":\"\\u0041\"}").parse();
    assert(parsed["letter"].asString() == "A");
}

void testRejectsInvalidStringsAndNumbers() {
    expectThrows([] {
        JsonParser parser("{\"bad\":\"line1\nline2\"}");
        parser.parse();
    }, "Unescaped control character", "testRejectsInvalidStringsAndNumbers/newline");

    expectThrows([] {
        JsonParser parser("{\"bad\":01}");
        parser.parse();
    }, "Leading zeroes", "testRejectsInvalidStringsAndNumbers/leading-zero");

    expectThrows([] {
        JsonParser parser("{\"bad\":1e+}");
        parser.parse();
    }, "Invalid exponent", "testRejectsInvalidStringsAndNumbers/exponent");
}

} // namespace

int main() {
    testLoadAndSaveRoundTrip();
    testRejectsTrailingContent();
    testEscapesStringsOnSerialize();
    testUnicodeEscapesParse();
    testRejectsInvalidStringsAndNumbers();

    std::cout << "All tests passed.\n";
    return 0;
}
