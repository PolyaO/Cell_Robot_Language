#pragma once

#include <vector>

#include "interpreter/rvals/rval.hpp"
#include "var/var.hpp"
namespace ast {
class Idx {
   public:
    Idx(unsigned idx, std::vector<unsigned> &&dim, unsigned line);
    var::var_type execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    std::vector<unsigned> _dim;
    unsigned _idx;
    unsigned _line;
};
}  // namespace ast
