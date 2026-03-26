#pragma once

#include <variant>

#include "backend/rvals/var/bool.hpp"
#include "backend/variant_alternative.hpp"
namespace var {
template<class T> class Var;
using var_type = std::variant<Var<int>, Var<bool_t>>;
}

namespace ast {
struct ExecCtx;
class Unary;
class Binary;
class Res;
class Ref;
class Env;
class Idx;
class Reduce;
class Extend;
using rval =
    std::variant<var::var_type, Unary, Binary, Res, Ref, Env, Idx, Reduce, Extend>;
template <typename T>
concept RvalType =
    requires(T t) { requires is_variant_alternative<rval, T>(); };
}  // namespace ast
