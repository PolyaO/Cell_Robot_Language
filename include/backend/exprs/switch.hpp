#pragma once

#include "backend/exprs/expr.hpp"

namespace ast {

class Switch {
   public:
    Switch(unsigned condition_idx, unsigned expr_true, unsigned expr_false,
           unsigned line);
    unsigned get_line() const noexcept;
    expr *execute(ExecCtx &ctx);

   private:
    unsigned _expr_true;
    unsigned _expr_false;
    unsigned _condition_idx;
    unsigned _line;
    bool _is_executed = false;
};
}  // namespace ast
