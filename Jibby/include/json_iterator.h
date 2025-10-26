#ifndef JIBBY_JSON_ITERATOR_H
#define JIBBY_JSON_ITERATOR_H

#include "json_types.h"

namespace jibby{

    class JsonIterator {
        public:
            // Typedefs fro convenience
            using ObjIter    = Object::iterator;
            using ArrIter    = Array::iterator;
            using CObjIter   = Object::const_iterator;
            using CArrIter   = Array::const_iterator;

            // Variant can hold any one of the typedefs
            using IterVariant = std::variant<ObjIter, ArrIter, CObjIter, CArrIter>;

            // Constructor overloads for each kind
            JsonIterator(ObjIter it);
            JsonIterator(ArrIter it);
            JsonIterator(CObjIter it);
            JsonIterator(CArrIter it);

            // Increment
            JsonIterator& operator++();

            // Inequality
            bool operator!=(const JsonIterator& other) const;

            // Dereference for mutable iteration
            std::pair<string, Json&> operator*();

            // Dereference for const iteration
            std::pair<string, const Json&> operator*() const;

        private:
            IterVariant iter;
    };

}
#endif