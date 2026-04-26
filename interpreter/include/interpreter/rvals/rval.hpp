#pragma once

#include <variant>

#include "interpreter/variant_alternative.hpp"
namespace exec {
struct GlobalCtx;
}
namespace ast {
class Unary;
class Binary;
class Res;
class Ref;
class Env;
class Idx;
class Reduce;
class Extend;
using rval =
    std::variant<Unary, Binary, Res, Ref, Env, Idx, Reduce, Extend>;
template <typename T>
concept RvalType =
    requires(T t) { requires is_variant_alternative<rval, T>(); };
}  // namespace ast
