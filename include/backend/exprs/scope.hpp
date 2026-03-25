#pragma once

#include <vector>

#include "backend/exprs/expr.hpp"

namespace ast {

class Scope {
   private:
    std::vector<unsigned> _exprs;
    unsigned _next_expr_idx = 0;
    unsigned _line;

   public:
    Scope(std::vector<unsigned> &&exprs, unsigned line);
    unsigned get_line() const noexcept;
    expr *execute(ExecCtx &ctx);
};
}  // namespace ast
