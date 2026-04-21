#pragma once
#include <variant>
#include "backend/variant_alternative.hpp"


namespace ast {
    struct ExecCtx;
    class Assign;
    class AssignVal;
    class Logitize;
    class Digitize;
    class Move;
    class RotateR;
    class RotateL;
    class Do;
    class For;
    class Switch;
    class Scope;
    class Task;
    using expr = std::variant<
        Assign,
        AssignVal,
        Logitize,
        Digitize,
        Move,
        RotateR,
        RotateL,
        Do,
        For,
        Switch,
        Scope,
        Task
    >;

    template <typename T>
    concept ExprType = requires(T t) {
        requires is_variant_alternative<expr, T>();
    };

}
