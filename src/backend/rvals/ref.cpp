#include <stdexcept>
#include <variant>

#include "backend/rvals.hpp"
#include "backend/rvals/rval.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Ref::Ref(unsigned idx, unsigned line) : _idx(idx), _line(line) {}

var::var_type ast::Ref::execute(ExecCtx &ctx) const {
    try {
        return ast::execute(ctx, ctx.ast.get_rval(_idx));
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("{} {}", _line, e.what()));
    }
}

var::var_type &ast::Ref::get(ExecCtx &ctx) const {
    auto &r = ctx.ast.get_rval(_idx);
    if (!std::holds_alternative<var::var_type>(r)) {
        throw InterpreterRuntimeError(std::format(
            "{} Rval of type Ref is not pointing to variable", _line));
    } else {
        return std::get<var::var_type>(r);
    }
}

unsigned ast::Ref::get_line() const noexcept { return _line; }
unsigned ast::Ref::get_idx() const noexcept { return _idx; }

void ast::Ref::set_idx(unsigned idx) noexcept { _idx = idx; }

