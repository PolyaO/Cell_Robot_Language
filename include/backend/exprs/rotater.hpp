#pragma once

#include "backend/exprs/expr.hpp"
namespace ast {
class RotateR {
   private:
    unsigned _line;
    bool _is_politely_asked = false;

   public:
    RotateR(unsigned line);
    void set_politely_asked() noexcept;
    bool is_politely_asked() const noexcept;
    unsigned get_line() const noexcept;
    expr *execute(ExecCtx &ctx);
};
}  // namespace ast
