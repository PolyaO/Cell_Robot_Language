#pragma once

#include <variant>

#include "backend/variant_alternative.hpp"
namespace ast {
struct ExecCtx;
class Var;
class Unary;
class Binary;
class Res;
class Ref;
class Env;
class Idx;
class Reduce;
class Extend;
using rval =
    std::variant<Var, Unary, Binary, Res, Ref, Env, Idx, Reduce, Extend>;
template <typename T>
concept RvalType =
    requires(T t) { requires is_variant_alternative<rval, T>(); };
}  // namespace ast
