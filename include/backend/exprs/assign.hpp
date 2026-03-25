#pragma once


#include "backend/exprs/expr.hpp"

namespace ast {
class Assign {
   private:
    unsigned _idx1;
    unsigned _idx2;
    unsigned _line;
    bool _is_politely_asked = false;

   public:
    Assign(unsigned idx1, unsigned idx2, unsigned line);
    void set_politely_asked() noexcept;
    bool is_politely_asked() const noexcept;
    unsigned get_line() const noexcept;
    expr *execute(ExecCtx &ctx);
};
}  // namespace ast
