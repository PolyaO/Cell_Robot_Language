#pragma once
#include "var/var.hpp"
#include "interpreter/exec/global_ctx.hpp"

namespace exec {
    var::var_type execute_rval(GlobalCtx &ctx, unsigned rval_idx);
}
