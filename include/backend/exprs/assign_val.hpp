#pragma once

#include "backend/exprs/expr.hpp"
#include "backend/rvals/var/bool.hpp"
#include <variant>

namespace ast {

class AssignVal {
   public:
    AssignVal(unsigned idx, std::variant<int, bool_t> val, unsigned line);
    void set_politely_asked() noexcept;
    bool is_politely_asked() const noexcept;
    expr *execute(ExecCtx &ctx);
    unsigned get_line() const noexcept;

   private:
    unsigned _idx;
    std::variant<int, bool_t> _val;
    unsigned _line;
    bool _is_politely_asked = false;
};

}  // namespace ast
