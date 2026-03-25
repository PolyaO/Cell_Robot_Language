#pragma once

#include "backend/rvals/rval.hpp"

namespace ast {
class Reduce {
   private:
    unsigned _rval_idx;
    unsigned _dim;

   public:
    Reduce(unsigned _rval_idx, unsigned dim);

    Var execute(ExecCtx &ctx) const;
};
}  // namespace ast
