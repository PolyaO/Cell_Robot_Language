#include "backend/rvals/rval.hpp"

#include <stdexcept>
#include <variant>

#include "backend/rvals.hpp"
#include "backend/rvals/var/var_ops.hpp"

var::var_type ast::execute(ExecCtx &ctx, rval &r) {
    return std::visit(
        [&](auto &rv) -> var::var_type {
            if constexpr (requires { rv.execute(ctx); })
                return rv.execute(ctx);
            else
                return var::execute(std::get<var::var_type>(r));
        },
        r);
}

var::var_type &ast::get_var(ExecCtx &ctx, rval &r) {
    if (std::holds_alternative<var::var_type>(r))
        return std::get<var::var_type>(r);
    return std::visit(
        [&](auto &v) -> var::var_type & {
            if constexpr (requires { v.get(ctx); })
                return v.get(ctx);
            else
                throw std::runtime_error(std::format(
                    "Trying to get var from rval with different type"));
        },
        r);
}
