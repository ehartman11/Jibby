#include "json_iterator.h"
#include "json_exception.h"
#include "json.h"
    
namespace jibby {

    JsonIterator::JsonIterator(ObjIter it)  : iter(it) {}
    JsonIterator::JsonIterator(ArrIter it)  : iter(it) {}
    JsonIterator::JsonIterator(CObjIter it) : iter(it) {}
    JsonIterator::JsonIterator(CArrIter it) : iter(it) {}

    // increment
    JsonIterator& JsonIterator::operator++() {
        std::visit([](auto& it){ ++it; }, iter);
        return *this;
    }

    // inequality
    bool JsonIterator::operator!=(const JsonIterator& other) const {
        // must be same type of iterator in both
        if (iter.index() != other.iter.index()) return true;
        return std::visit([&](auto& it) {
            using T = std::decay_t<decltype(it)>;
            return it != std::get<T>(other.iter);
        }, iter);
    }

    // dereference fuyor mutable iteration
    std::pair<string, Json&> JsonIterator::operator*() {
        return std::visit([](auto& it) -> std::pair<string, Json&> {
            using T = std::decay_t<decltype(it)>;

            if constexpr (std::is_same_v<T, ObjIter>)
                return {it->first, it->second};
            else if constexpr (std::is_same_v<T, ArrIter>) 
                return {"", *it};
            else
                throw JsonException("Cannot dereference const iterator with non-const operator*()");
        }, iter);
    }

    // dereference for const iteration
    std::pair<string, const Json&> JsonIterator::operator*() const {
        return std::visit([](auto& it) -> std::pair<string, const Json&> {
            using T = std::decay_t<decltype(it)>;

            if constexpr (std::is_same_v<T, CObjIter>)
                return {it->first, it->second};
            else if constexpr (std::is_same_v<T, CArrIter>)
                return {"", *it};
            else
                throw JsonException("Cannot dereference mutable iterator with const operator*()");
        }, iter);
    }
}