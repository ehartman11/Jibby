#include "json_io.h"
#include "json_exception.h"
#include "json_parser.h"
#include <fstream>
#include <sstream>

namespace jibby {
    // Read in a json file from source: filepath
    Json JsonIO::read(const string& filepath) {
        std::ifstream file(filepath);
        // Check file is open, if not throw an error message
        if (!file.is_open()) {
            throw JsonException("Failed to open file for reading: " + filepath);
        }

        // create a stream variable to hold the entire string from the json file
        std::stringstream buffer;
        // read and assign the file contents to the buffer variable
        buffer << file.rdbuf();
        // close the file
        file.close();
        
        // parse the json buffer. Throw error if encountered
        try{
            JsonParser parser(buffer.str());
            return parser.parse();
        } catch (const JsonException& e) {
            throw;
        } catch (const std::exception& e) {
            throw JsonException("Error while parsing file: " + filepath + " | " + e.what());
        }
    }

    // Write to a json file, include prettifying the structure if desired
    void JsonIO::write(const Json& json, const string& filepath, bool pretty) {
        // Output file stream object using the desired filepath
        std::ofstream file(filepath);
        // Verify the file opens properly
        if (!file.is_open()) {
            throw JsonException("Failed to open file for writing: " + filepath);
        }

        // Write the serialized json object to the file (4 is yes, 0 is no) 
        file << json.serialize(pretty ? 4 : 0);
        // Close the file
        file.close();

        //
        if (!file) {
            throw JsonException("Error occurred while writing file: " + filepath);
        }
    }

}