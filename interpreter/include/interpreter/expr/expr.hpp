#pragma once
#include <variant>

#include "interpreter/expr/exprs.hpp"
#include "interpreter/variant_alternative.hpp"

namespace ast {

class Expr {
   public:
    using expr_t =
        std::variant<exprs::Assign, exprs::Declare, exprs::AssignVal,
                     exprs::Logitize, exprs::Digitize, exprs::Move,
                     exprs::RotateR, exprs::RotateL, exprs::Do, exprs::For,
                     exprs::Switch, exprs::Scope, exprs::RetRes>;
    Expr(unsigned lineno, bool is_scopelike_expr, expr_t&& expr);
    Expr* execute(exec::GlobalCtx& ctx) const;
    unsigned get_lineno() const noexcept;
    void set_politely_asked() noexcept;
    bool is_politely_asked() const noexcept;
    bool is_scopelike_expr() const noexcept;
    void set_task_idx(unsigned task_idx) noexcept;

   private:
    bool _is_scopelike_expr;
    bool _is_politely_asked = false;
    unsigned _lineno;
    expr_t _expr;
};

template <typename T>
concept ExprType =
    requires(T t) { requires is_variant_alternative<Expr::expr_t, T>(); };

}  // namespace ast
