#include "interpreter/rvals/env.hpp"

// #include "interpreter/exceptions/runtime_exceptions.hpp"
// #include "interpreter/exec/execute.hpp"
#include <iostream>

#include "interpreter/exec/global_ctx.hpp"
// #include "var/var.hpp"

namespace ast {
Env::Env(unsigned line) : _line(line) {}
var::var_type Env::execute(exec::GlobalCtx &ctx) const {
    return ctx.robot->get_env();
}
unsigned Env::get_line() const noexcept { return _line; }
}  // namespace ast
