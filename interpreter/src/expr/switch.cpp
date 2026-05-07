#include <stdexcept>

#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/expr.hpp"
#include "interpreter/expr/exprs.hpp"
#include "var/var_ops.hpp"
namespace ast::exprs {
Switch::Switch(unsigned condition_idx, unsigned is_executed_idx,
               unsigned expr_true, unsigned expr_false)
    : _condition_idx(condition_idx),
      _is_executed_idx(is_executed_idx),
      _expr_true(expr_true),
      _expr_false(expr_false) {}

Expr *Switch::execute(exec::GlobalCtx &ctx) const {
    auto &curr_ctx = ctx.get_curr_ctx();
    if (curr_ctx.get_counter(_is_executed_idx)) {
        curr_ctx.set_counter(0, _is_executed_idx);
        return nullptr;
    }
    curr_ctx.set_counter(1, _is_executed_idx);
    try {
        if (var::equal(ctx.ast.get_rval(_condition_idx)->execute(ctx),
                       var::TRUE) &&
            _expr_true != -1)
            return ctx.ast.get_expr(_expr_true);
        if (var::equal(ctx.ast.get_rval(_condition_idx)->execute(ctx),
                       var::FALSE) &&
            _expr_false != -1)
            return ctx.ast.get_expr(_expr_false);
        curr_ctx.set_counter(0, _is_executed_idx);
        return nullptr;
    } catch (const std::runtime_error &e) {
        throw std::runtime_error(
            std::format("{}: {}", e.what(),
                        "In switch condition result should be TRUE or FALSE "
                        "(boolean matrix dim [1])"));
    }
}
}  // namespace ast::exprs
