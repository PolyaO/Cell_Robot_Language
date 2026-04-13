#include "backend/exprs/expr.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Scope::Scope(std::vector<unsigned> &&exprs, unsigned line)
    : _exprs(std::move(exprs)), _line(line) {}

ast::expr *ast::Scope::execute(ExecCtx &ctx) {
    if (_next_expr_idx == _exprs.size()) {
        _next_expr_idx = 0;
        return nullptr;
    }
    return ctx.ast.get_expr(_exprs[_next_expr_idx++]);
}
unsigned ast::Scope::get_line() const noexcept { return _line; }
