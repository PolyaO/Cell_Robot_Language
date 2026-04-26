#include <stdexcept>

#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/execute.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "var/var_ops.hpp"

namespace ast {

Digitize::Digitize(unsigned idx, unsigned line) : _idx(idx), _line(line) {}

bool Digitize::is_politely_asked() const noexcept { return _is_politely_asked; }

void Digitize::set_politely_asked() noexcept { _is_politely_asked = true; }

expr *Digitize::execute(exec::GlobalCtx &ctx) const {
    try {
        var::digitize(exec::execute_rval(ctx, _idx));
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("[{}] {}", _line, e.what()));
    }
    return nullptr;
}

unsigned Digitize::get_line() const noexcept { return _line; }
}  // namespace ast
