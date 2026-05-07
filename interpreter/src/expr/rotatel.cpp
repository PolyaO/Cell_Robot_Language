#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/expr.hpp"
#include "interpreter/expr/exprs.hpp"
namespace ast::exprs {
Expr *RotateL::execute(exec::GlobalCtx &ctx) const {
    ctx.robot->rotate_l();
    return nullptr;
}
}  // namespace ast::exprs
