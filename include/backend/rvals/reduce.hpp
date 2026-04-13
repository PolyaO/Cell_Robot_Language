#pragma once

#include "backend/rvals/rval.hpp"

namespace ast {
class Reduce {
   public:
    Reduce(unsigned _idx, unsigned dim, unsigned change, unsigned line);

    var::var_type execute(ExecCtx &ctx) const;
    unsigned get_line();

   private:
    unsigned _idx;
    unsigned _dim;
    unsigned _change;
    unsigned _line;
};
}  // namespace ast
