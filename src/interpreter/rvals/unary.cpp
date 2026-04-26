#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/execute.hpp"
#include "interpreter/exec/global_ctx.hpp"
namespace ast {
Unary::Unary(unsigned idx, Unary::Op op, unsigned line)
    : _idx(idx), _op(std::move(op)), _line(line) {}

var::var_type Unary::execute(exec::GlobalCtx &ctx) const {
    try {
        return _op(exec::execute_rval(ctx, _idx));
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("[{}] {}", _line, e.what()));
    }
}
unsigned Unary::get_line() const noexcept { return _line; }

}  // namespace ast
