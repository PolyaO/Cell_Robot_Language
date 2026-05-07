#include "interpreter/expr/expr.hpp"

#include <variant>

#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/exprs.hpp"
#include "interpreter/rval/rval.hpp"

namespace ast {
Expr::Expr(unsigned lineno, bool is_scopelike_expr, expr_t &&expr)
    : _lineno(lineno),
      _is_scopelike_expr(is_scopelike_expr),
      _expr(std::move(expr)) {}

Expr *Expr::execute(exec::GlobalCtx &ctx) const {
    try {
        Expr *res =
            std::visit([&ctx](auto &e) { return e.execute(ctx); }, _expr);
        if (!_is_scopelike_expr &&
            !std::holds_alternative<exprs::Switch>(_expr)) {
            ctx.robot->ask(_is_politely_asked);
        }
        return res;
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(
            std::format("[{}] {}", _lineno, e.what()));
    }
}
void Expr::set_politely_asked() noexcept { _is_politely_asked = true; }
bool Expr::is_politely_asked() const noexcept { return _is_politely_asked; }
bool Expr::is_scopelike_expr() const noexcept { return _is_scopelike_expr; }
void Expr::set_task_idx(unsigned task_idx) noexcept {
    std::visit(
        [task_idx](auto &e) {
            if constexpr (requires { e.set_task_idx(task_idx); }) {
                e.set_task_idx(task_idx);
            }
        },
        _expr);
}
unsigned Expr::get_lineno() const noexcept { return _lineno; }
}  // namespace ast
