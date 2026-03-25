#pragma once

#include "backend/rvals/rval.hpp"
#include "backend/rvals/var.hpp"
namespace ast {
class Idx {
   private:
    std::vector<unsigned> _dim_list;
    unsigned _rval_idx;

   public:
    Idx( unsigned rval_idx, std::vector<unsigned> &&dim_list);
    Var execute(ExecCtx &ctx) const;
};
}  // namespace ast
