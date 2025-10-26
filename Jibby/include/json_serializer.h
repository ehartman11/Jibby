#ifndef JIBBY_JSON_SERIALIZER_H
#define JIBBY_JSON_SERIALIZER_H

#include "json.h"

namespace jibby {

    class JsonSerializer {
        public:
            static string serialize(const Json& value, int indent = 0);
    };

}

#endif 
