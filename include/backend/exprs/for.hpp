#pragma once

#include "backend/exprs/expr.hpp"
#include "backend/rvals/var/var.hpp"

namespace ast {

class For {
   public:
    For(unsigned counter, unsigned boundary,
        unsigned step, unsigned stmt, unsigned line);
    unsigned get_line() const noexcept;
    expr *execute(ExecCtx &ctx);

   private:
    unsigned _stmt;
    unsigned _counter;
    unsigned _reserve_counter;
    unsigned _boundary;
    unsigned _step;
    unsigned _line;
    bool _is_first_step = true;
    short _step_number = 0;
    void check_for_same_dims(var::Var<int> &a, var::Var<int> &b,
                             var::Var<int> &c);
};
}  // namespace ast
