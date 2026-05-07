#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/expr.hpp"
#include "interpreter/expr/exprs.hpp"
namespace ast::exprs {
Expr *RotateR::execute(exec::GlobalCtx &ctx) const {
    ctx.robot->rotate_r();
    return nullptr;
}
}  // namespace ast::exprs
