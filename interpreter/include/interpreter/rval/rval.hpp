#pragma once
#include <variant>

#include "interpreter/rval/rvals.hpp"
#include "interpreter/variant_alternative.hpp"
#include "var/var.hpp"
namespace exec {
class GlobalCtx;
}

namespace ast {

class Rval {
   public:
    using rval_t =
        std::variant<rvals::Unary, rvals::Binary, rvals::Res, rvals::Ref,
                     rvals::Env, rvals::Idx, rvals::Reduce, rvals::Extend>;

    Rval(unsigned lineno, rval_t &&rval);
    var::var_type execute(exec::GlobalCtx &ctx) const;
    unsigned get_lineno() const noexcept;

   private:
    unsigned _lineno;
    rval_t _rval;
};

template <typename T>
concept RvalType =
    requires(T t) { requires is_variant_alternative<Rval::rval_t, T>(); };
}  // namespace ast
