#include "interpreter/rvals/ref.hpp"

#include "interpreter/exec/global_ctx.hpp"
#include "var/var_ops.hpp"

namespace ast {
Ref::Ref(unsigned idx_in_ctx, unsigned line)
    : _idx_in_ctx(idx_in_ctx), _line(line) {}

var::var_type Ref::execute(exec::GlobalCtx &ctx) const {
    return var::copy_ref(*ctx.get_curr_ctx().get_var(_idx_in_ctx));
}

unsigned Ref::get_line() const noexcept { return _line; }
}  // namespace ast
