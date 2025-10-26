#ifndef JIBBY_IO_H
#define JIBBY_IO_H

#include "json.h"
#include <string>

namespace jibby {

    class JsonIO {
        public:
            // Read from file
            static Json read(const string& filepath);

            // Write to file
            static void write(const Json& json, const string& filepath, bool pretty=false);

    };

} 

#endif 