#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/exprs.hpp"
#include "var/var_ops.hpp"
namespace ast::exprs {
AssignVal::AssignVal(unsigned idx, std::variant<int, bool_t> &&val)
    : _idx(idx), _val(val) {}

Expr *AssignVal::execute(exec::GlobalCtx &ctx) const {
    if (std::holds_alternative<int>(_val))
        var::assign_val<int>(ctx.ast.get_rval(_idx)->execute(ctx),
                             std::get<int>(_val));
    else
        var::assign_val<bool_t>(ctx.ast.get_rval(_idx)->execute(ctx),
                                std::get<bool_t>(_val));
    return nullptr;
}
}  // namespace ast::exprs
