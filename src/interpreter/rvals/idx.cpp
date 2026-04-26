#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/execute.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "var/var_ops.hpp"

namespace ast {
Idx::Idx(unsigned idx, std::vector<unsigned> &&dim, unsigned line)
    : _idx(idx), _dim(std::move(dim)), _line(line) {}

var::var_type Idx::execute(exec::GlobalCtx &ctx) const {
    try {
        return var::idx(exec::execute_rval(ctx, _idx), _dim);
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("[{}] {}", _line, e.what()));
    }
}
unsigned Idx::get_line() const noexcept { return _line; }
}  // namespace ast
