#pragma once


#include "backend/exprs/expr.hpp"

namespace ast {

class For {
   private:
    unsigned _stmt;
    unsigned _counter;
    unsigned _curr_counter;
    unsigned _boundary;
    unsigned _step;
    unsigned _line;
    bool _is_first_step = true;

   public:
    For(unsigned counter, unsigned curr_counter, unsigned boundary,
        unsigned step, unsigned stmt, unsigned line);
    unsigned get_line() const noexcept;
    expr *execute(ExecCtx &ctx);
};
}  // namespace ast
