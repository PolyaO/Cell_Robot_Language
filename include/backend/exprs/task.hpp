#pragma once
#include <memory>
#include <vector>

#include "backend/exprs/expr.hpp"
#include "backend/rvals/rval.hpp"

namespace ast {
class Task {
   public:
    Task(std::vector<unsigned> &&exprs, unsigned res_idx, unsigned args_number,
         unsigned line);
    unsigned get_res_idx() const noexcept;
    unsigned get_args_number() const noexcept;
    unsigned get_line() const noexcept;
    expr *execute(ExecCtx &ctx);

   private:
    std::unique_ptr<unsigned[]> _exprs;
    unsigned _exprs_size;
    unsigned _next_expr_idx = 0;
    unsigned _res_idx;
    unsigned _line;
    unsigned _args_num;

};
}  // namespace ast
