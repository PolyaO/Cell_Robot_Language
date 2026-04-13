#include <stdexcept>

#include "backend/rvals.hpp"
#include "backend/rvals/rval.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Unary::Unary(unsigned idx, Unary::Op op, unsigned line)
    : _idx(idx), _op(std::move(op)), _line(line) {}

var::var_type ast::Unary::execute(ExecCtx &ctx) const {
    try {
        return _op(ast::execute(ctx, ctx.ast.get_rval(_idx)));
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("{} {}", _line, e.what()));
    }
}
unsigned ast::Unary::get_line() { return _line; }
