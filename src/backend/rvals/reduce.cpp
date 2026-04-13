#include <stdexcept>

#include "backend/rvals.hpp"
#include "backend/rvals/rval.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Reduce::Reduce(unsigned idx, unsigned dim, unsigned change, unsigned line)
    : _idx(idx), _dim(dim), _change(change), _line(line) {}

var::var_type ast::Reduce::execute(ExecCtx &ctx) const {
    try {
        return var::reduce(ast::execute(ctx, ctx.ast.get_rval(_idx)), _dim,
                           _change);
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("{} {}", _line, e.what()));
    }
}
unsigned ast::Reduce::get_line() { return _line; }
