#pragma once

#include "backend/rvals/rval.hpp"
namespace ast {
class Ref {
   public:
    Ref(unsigned rval_idx, unsigned line);
    var::var_type execute(ExecCtx &ctx) const;
    unsigned get_line();

   private:
    unsigned _rval_idx;
    unsigned _line;
};
}  // namespace ast
