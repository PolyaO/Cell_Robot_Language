#pragma once

#include <vector>

#include "backend/rvals/rval.hpp"
namespace ast {
class Idx {
   public:
    Idx(unsigned idx, std::vector<unsigned> &&dim, unsigned line);
    var::var_type execute(ExecCtx &ctx) const;
    unsigned get_line() const noexcept;
    const std::vector<unsigned> &get_dim() const noexcept;

   private:
    std::vector<unsigned> _dim;
    unsigned _idx;
    unsigned _line;
};
}  // namespace ast
