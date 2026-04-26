#pragma once
#include <variant>

#include "interpreter/variant_alternative.hpp"

namespace exec {
struct GlobalCtx;
}

namespace ast {
class Assign;
class Declare;
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
using expr = std::variant<Assign, Declare, AssignVal, Logitize, Digitize, Move,
                          RotateR, RotateL, Do, For, Switch, Scope, Task>;

template <typename T>
concept ExprType =
    requires(T t) { requires is_variant_alternative<expr, T>(); };

}  // namespace ast
