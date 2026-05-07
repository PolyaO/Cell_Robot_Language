#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/exprs.hpp"
#include "var/var_ops.hpp"
namespace ast::exprs {

Assign::Assign(unsigned idx1, unsigned idx2) : _idx1(idx1), _idx2(idx2) {}

Expr *Assign::execute(exec::GlobalCtx &ctx) const {
    var::assign(ctx.ast.get_rval(_idx1)->execute(ctx),
                ctx.ast.get_rval(_idx2)->execute(ctx));
    return nullptr;
}
}  // namespace ast::exprs
