#pragma once

#include <vector>

#include "backend/rvals/rval.hpp"
namespace ast {
class Idx {
   public:
    Idx(unsigned rval_idx, std::vector<unsigned> &&dim_list, unsigned line);
    var::var_type execute(ExecCtx &ctx) const;
    unsigned get_line();

   private:
    std::vector<unsigned> _dim_list;
    unsigned _rval_idx;
    unsigned _line;
};
}  // namespace ast
