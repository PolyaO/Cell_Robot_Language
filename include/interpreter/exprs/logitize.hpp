#pragma once

#include "interpreter/exprs/expr.hpp"

namespace ast {
class Logitize {
   public:
    Logitize(unsigned idx, unsigned line);
    void set_politely_asked() noexcept;
    bool is_politely_asked() const noexcept;
    unsigned get_line() const noexcept;
    expr *execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _idx;
    unsigned _line;
    bool _is_politely_asked = false;
};
}  // namespace ast
