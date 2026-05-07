#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/expr.hpp"
#include "interpreter/expr/exprs.hpp"
namespace ast::exprs {
Scope::Scope(std::vector<unsigned> &&exprs, unsigned counter_idx)
    : _exprs(std::move(exprs)), _counter_idx(counter_idx) {}

Expr *Scope::execute(exec::GlobalCtx &ctx) const {
    auto &curr_ctx = ctx.get_curr_ctx();
    unsigned next_expr_idx = curr_ctx.get_counter(_counter_idx);
    if (_exprs.size() == next_expr_idx) {
        curr_ctx.set_counter(0, _counter_idx);
        return nullptr;
    }
    curr_ctx.increment_counter(_counter_idx);
    return ctx.ast.get_expr(_exprs[next_expr_idx]);
}
}  // namespace ast::exprs
