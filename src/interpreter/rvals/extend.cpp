#include "interpreter/rvals/extend.hpp"

#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/execute.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "var/var_ops.hpp"

namespace ast {
Extend::Extend(unsigned idx, unsigned dim, unsigned change, unsigned line)
    : _idx(idx), _dim(dim), _change(change), _line(line) {}

var::var_type Extend::execute(exec::GlobalCtx &ctx) const {
    try {
        return var::extend(exec::execute_rval(ctx, _idx), _dim, _change);
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("[{}] {}", _line, e.what()));
    }
}
unsigned Extend::get_line() const noexcept { return _line; }

}  // namespace ast
