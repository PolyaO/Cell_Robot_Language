#include "interpreter/exec/execute.hpp"

#include "var/var.hpp"
namespace exec {
var::var_type execute_rval(GlobalCtx &ctx, unsigned rval_idx) {
    return std::visit(
        [&](auto &rv) -> var::var_type { return rv.execute(ctx); },
        ctx.ast.get_rval(rval_idx));
}
}  // namespace exec
