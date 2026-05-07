#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/expr.hpp"
#include "interpreter/expr/exprs.hpp"
namespace ast::exprs {
Expr *Move::execute(exec::GlobalCtx &ctx) const {
    ctx.robot->move();
    return nullptr;
}
}  // namespace ast::exprs
