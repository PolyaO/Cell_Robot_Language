#pragma once

#include "interpreter/exec/local_ctx.hpp"
#include "interpreter/exprs/expr.hpp"
#include "var/var.hpp"

namespace ast {

class For {
   public:
    For(unsigned counter_idx, unsigned boundary_idx, unsigned step_idx,
        unsigned stmt_idx, unsigned reserve_counter_idx,
        unsigned dim_number_idx, unsigned is_first_step_idx, unsigned line);
    unsigned get_line() const noexcept;
    expr *execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _counter_idx;
    unsigned _boundary_idx;
    unsigned _step_idx;
    unsigned _stmt_idx;

    unsigned _reserve_counter_idx;
    unsigned _dim_number_idx;
    unsigned _is_first_step_idx;
    unsigned _line;

    void throw_if_diff_dims(var::Var<int> &a, var::Var<int> &b,
                            var::Var<int> &c) const;
    expr *first_step_case(exec::GlobalCtx &ctx) const;
    expr *last_step_case(exec::GlobalCtx &ctx) const;
};
}  // namespace ast
