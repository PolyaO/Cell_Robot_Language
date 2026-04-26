#pragma once
#include <variant>

#include "interpreter/exprs/expr.hpp"
#include "var/bool.hpp"

namespace ast {

class AssignVal {
   public:
    AssignVal(unsigned idx, std::variant<int, bool_t> &&val, unsigned line);
    void set_politely_asked() noexcept;
    bool is_politely_asked() const noexcept;
    expr *execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _idx;
    std::variant<int, bool_t> _val;
    unsigned _line;
    bool _is_politely_asked = false;
};

}  // namespace ast
