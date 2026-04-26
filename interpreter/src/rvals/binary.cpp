#include "interpreter/rvals/binary.hpp"

#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/execute.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "var/var.hpp"

namespace ast {
Binary::Binary(unsigned idx1, unsigned idx2, Binary::Op op, unsigned line)
    : _idx1(idx1), _idx2(idx2), _op(std::move(op)), _line(line) {}

var::var_type Binary::execute(exec::GlobalCtx &ctx) const {
    try {
        return _op(exec::execute_rval(ctx, _idx1),
                   exec::execute_rval(ctx, _idx2));
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("[{}] {}", _line, e.what()));
    }
}
unsigned Binary::get_line() const noexcept { return _line; }
}  // namespace ast

