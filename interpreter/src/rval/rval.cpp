#include "interpreter/rval/rval.hpp"

#include "interpreter/exceptions/runtime_exceptions.hpp"

namespace ast {
Rval::Rval(unsigned lineno, rval_t &&rval)
    : _lineno(lineno), _rval(std::move(rval)) {}
var::var_type Rval::execute(exec::GlobalCtx &ctx) const {
    try {
        return std::visit([&ctx](auto &rv) { return rv.execute(ctx); }, _rval);
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(
            std::format("[{}] {}", _lineno, e.what()));
    }
}
unsigned Rval::get_lineno() const noexcept { return _lineno; }
}  // namespace ast
