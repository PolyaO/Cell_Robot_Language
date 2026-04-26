#pragma once

#include <functional>
#include "var/var.hpp"
#include "interpreter/rvals/rval.hpp"

namespace ast {
class Binary {
   public:
    using Op = std::function<var::var_type(const var::var_type &,
                                           const var::var_type &)>;

    Binary(unsigned idx1, unsigned idx2, Op op, unsigned line);

    var::var_type execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _idx1;
    unsigned _idx2;
    unsigned _line;
    Op _op;
};
}  // namespace ast
