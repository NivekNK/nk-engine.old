#pragma once

#include "core/map_macro.h"

namespace nk {
#define NK_ERROR_ENUM_VALUE(Value) Value
#define DefineEnumError(Name, Values)                             \
    enum class Name {                                             \
        Ok,                                                       \
        MAP_LIST(NK_ERROR_ENUM_VALUE, NK_EXPAND_ARGUMENTS Values) \
    }

    // template <typename V, typename E>
    // using Result = std::variant<V, E>;

    template <typename V, typename E>
    struct Result {
        std::optional<V> value;
        E err = static_cast<E>(0);

        Result(V value) : value{value} {}
        Result(E err) : err{err} {}

        using ResultValue = V;
        using ResultError = E;
    };

    class Test {};
    DefineEnumError(Error, (
        Error1,
        Error2,
    ));

    void function() {
        Result<Test*, Error> test = Error::Error1;
    }


#define match(Type, Error, Value, ...)              \
    {                                               \
        Error err = Error::Ok;                      \
        if (std::holds_alternative<Error>(Value)) { \
            err = std::get<Error>(Value);           \
        }                                           \
        switch (err)                                \
            __VA_ARGS__                             \
    }
}
