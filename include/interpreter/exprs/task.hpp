#pragma once
#include <vector>

#include "interpreter/exprs/expr.hpp"
#include "interpreter/rvals/rval.hpp"

namespace ast {
class Task {
   public:
    Task(std::vector<unsigned> &&exprs, unsigned counter_idx, unsigned line);
    unsigned get_line() const noexcept;
    expr *execute(exec::GlobalCtx &ctx) const;

   private:
    std::vector<unsigned> _exprs;
    unsigned _counter_idx;
    unsigned _line;
};
}  // namespace ast
