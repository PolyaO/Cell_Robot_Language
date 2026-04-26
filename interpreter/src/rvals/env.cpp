#include "interpreter/rvals/env.hpp"

// #include "interpreter/exceptions/runtime_exceptions.hpp"
// #include "interpreter/exec/execute.hpp"
#include <iostream>

#include "interpreter/exec/global_ctx.hpp"
// #include "var/var.hpp"

namespace ast {
Env::Env(unsigned line) : _line(line) {}
var::var_type Env::execute(exec::GlobalCtx &ctx) const {
    std::cout << std::format("[{}] ROBOT GET_ENVIRONMENT\n", _line)
              << std::endl;
    return var::var_type(var::Var<bool_t>(false, {5, 5, 2}));
}
unsigned Env::get_line() const noexcept { return _line; }
}  // namespace ast
