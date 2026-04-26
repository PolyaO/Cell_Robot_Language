#pragma once
#include "interpreter/rvals/rval.hpp"
#include "var/var.hpp"

namespace ast {
class Extend {
   public:
    Extend(unsigned idx, unsigned dim, unsigned change, unsigned line);

    var::var_type execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _idx;
    unsigned _dim;
    unsigned _change;
    unsigned _line;
};
}  // namespace ast
