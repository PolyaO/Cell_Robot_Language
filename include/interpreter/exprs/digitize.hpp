#pragma once

#include "interpreter/exprs/expr.hpp"

namespace ast {

class Digitize {
   public:
    Digitize(unsigned idx, unsigned line);
    void set_politely_asked() noexcept;
    bool is_politely_asked() const noexcept;
    expr *execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _idx;
    unsigned _line;
    bool _is_politely_asked = false;
};
}  // namespace ast
