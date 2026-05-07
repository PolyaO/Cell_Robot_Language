#include "interpreter/rval/rvals.hpp"

#include "interpreter/exec/global_ctx.hpp"
#include "var/var_ops.hpp"
namespace ast::rvals {
Binary::Binary(unsigned idx1, unsigned idx2, Binary::Op op)
: _idx1(idx1), _idx2(idx2), _op(std::move(op)) {}

var::var_type Binary::execute(exec::GlobalCtx &ctx) const {
    return _op(ctx.ast.get_rval(_idx1)->execute(ctx),
               ctx.ast.get_rval(_idx2)->execute(ctx));
}

Unary::Unary(unsigned idx, Unary::Op op) : _idx(idx), _op(std::move(op)) {}

var::var_type Unary::execute(exec::GlobalCtx &ctx) const {
    return _op(ctx.ast.get_rval(_idx)->execute(ctx));
}

var::var_type Env::execute(exec::GlobalCtx &ctx) const {
    return ctx.robot->get_env();
}

Extend::Extend(unsigned idx, unsigned dim, unsigned change)
    : _idx(idx), _dim(dim), _change(change) {}

var::var_type Extend::execute(exec::GlobalCtx &ctx) const {
    return var::extend(ctx.ast.get_rval(_idx)->execute(ctx), _dim, _change);
}

Reduce::Reduce(unsigned idx, unsigned dim, unsigned change)
    : _idx(idx), _dim(dim), _change(change) {}

var::var_type Reduce::execute(exec::GlobalCtx &ctx) const {
    return var::reduce(ctx.ast.get_rval(_idx)->execute(ctx), _dim, _change);
}

Idx::Idx(unsigned idx, std::vector<unsigned> &&dim)
    : _idx(idx), _dim(std::move(dim)) {}

var::var_type Idx::execute(exec::GlobalCtx &ctx) const {
    return var::idx(ctx.ast.get_rval(_idx)->execute(ctx), _dim);
}

Ref::Ref(unsigned idx_in_ctx)
    : _idx_in_ctx(idx_in_ctx) {}

var::var_type Ref::execute(exec::GlobalCtx &ctx) const {
    return var::copy_ref(*ctx.get_curr_ctx().get_var(_idx_in_ctx));
}


Res::Res(unsigned task_idx) : _task_idx(task_idx) {}

var::var_type Res::execute(exec::GlobalCtx &ctx) const {
    auto metainf = *ctx.ast.find_task_metainf(_task_idx);
    return metainf.res;
}


}  // namespace ast::rvals
