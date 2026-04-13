#include <stdexcept>

#include "backend/rvals.hpp"
#include "backend/rvals/rval.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Idx::Idx(unsigned idx, std::vector<unsigned> &&dim, unsigned line)
    : _idx(idx), _dim(std::move(dim)), _line(line) {}

var::var_type ast::Idx::execute(ExecCtx &ctx) const {
    try {
        return var::idx(ast::execute(ctx, ctx.ast.get_rval(_idx)), _dim);
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("{} {}", _line, e.what()));
    }
}
unsigned ast::Idx::get_line() { return _line; }
