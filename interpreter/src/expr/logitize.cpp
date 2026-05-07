#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/exprs.hpp"
#include "var/var_ops.hpp"
namespace ast::exprs {
Logitize::Logitize(unsigned idx) : _idx(idx) {}

Expr *Logitize::execute(exec::GlobalCtx &ctx) const {
        var::logitize(ctx.ast.get_rval(_idx)->execute(ctx));
    return nullptr;
}
}  // namespace ast
