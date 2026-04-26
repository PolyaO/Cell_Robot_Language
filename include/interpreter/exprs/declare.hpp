#pragma once
#include <memory>
#include <variant>

#include "interpreter/exprs/expr.hpp"
#include "var/bool.hpp"
#include "var/var.hpp"

namespace ast {
class Declare {
   public:
    Declare(unsigned idx_in_ctx, const var::dim_t &dim,
            std::variant<int, bool_t> &&val, unsigned line);
    void set_politely_asked() noexcept;
    bool is_politely_asked() const noexcept;
    expr *execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    std::unique_ptr<unsigned[]> _dim;
    std::variant<int, bool_t> _val;
    unsigned _dim_size;
    unsigned _idx_in_ctx;
    bool _is_politely_asked = false;
    unsigned _line;
};
}  // namespace ast
