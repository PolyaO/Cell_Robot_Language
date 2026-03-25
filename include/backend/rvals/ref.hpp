#pragma once

#include "backend/rvals/rval.hpp"
#include "backend/rvals/var.hpp"
namespace ast {
class Ref {
   private:
    unsigned _rval_idx;
    unsigned _decl_line;

   public:
    Ref(unsigned line, unsigned rval_idx = 0);
    Var execute(ExecCtx &ctx) const;
    unsigned get_decl_line();
};
}  // namespace ast
