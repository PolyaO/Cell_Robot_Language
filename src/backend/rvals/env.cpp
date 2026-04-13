#include "backend/rvals/env.hpp"

#include <iostream>

#include "backend/rvals/rval.hpp"
#include "backend/rvals/var/var.hpp"

ast::Env::Env(unsigned line) : _line(line) {}
var::var_type ast::Env::execute(ExecCtx &ctx) const {
    std::cout << std::format("[{}] ROBOT GET_ENVIRONMENT", _line) << std::endl;
    return var::var_type(var::Var<bool_t>(false, {5, 5, 2}));
}
unsigned ast::Env::get_line() { return _line; }
