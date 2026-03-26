#pragma once

#include "backend/rvals/rval.hpp"
namespace ast {
class Ref {
   public:
    Ref(unsigned line, unsigned rval_idx = 0);
    var::var_type execute(ExecCtx &ctx) const;
    unsigned get_line();

   private:
    unsigned _rval_idx;
    unsigned _line;
};
}  // namespace ast
