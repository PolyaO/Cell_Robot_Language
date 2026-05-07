#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/exprs.hpp"
#include "var/var_ops.hpp"
namespace ast::exprs {
Digitize::Digitize(unsigned idx) : _idx(idx) {}

Expr *Digitize::execute(exec::GlobalCtx &ctx) const {
        var::digitize(ctx.ast.get_rval(_idx)->execute(ctx));
    return nullptr;
}
}  // namespace ast
