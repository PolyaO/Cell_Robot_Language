#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/expr.hpp"
#include "interpreter/expr/exprs.hpp"
#include "var/var_ops.hpp"
namespace ast::exprs {
RetRes::RetRes(unsigned idx_in_ctx) : _idx_in_ctx(idx_in_ctx) {}
Expr *RetRes::execute(exec::GlobalCtx &ctx) const {
    auto curr_ctx = ctx.get_curr_ctx();
    auto task_info_ptr =
        ctx.ast.find_task_metainf(ctx.get_curr_ctx().get_task_idx());
    task_info_ptr->res = std::move(var::copy(*curr_ctx.get_var(_idx_in_ctx)));
    return (Expr*)1;
}
}  // namespace ast::exprs
