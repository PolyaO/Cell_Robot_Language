#pragma once

#include <functional>

#include "var/var.hpp"
#include "interpreter/rvals/rval.hpp"

namespace ast {
class Unary {
   public:
    using Op = std::function<var::var_type(const var::var_type &)>;

    Unary(unsigned idx, Op op, unsigned line);

    var::var_type execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _idx;
    unsigned _line;
    Op _op;
};
}  // namespace ast
