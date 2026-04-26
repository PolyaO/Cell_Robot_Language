#include "interpreter/exprs/assign.hpp"

#include <interpreter/exprs.hpp>
#include <stdexcept>

#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/execute.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "var/var_ops.hpp"
namespace ast {

Assign::Assign(unsigned idx1, unsigned idx2, unsigned line)
    : _idx1(idx1), _idx2(idx2), _line(line) {}

bool Assign::is_politely_asked() const noexcept { return _is_politely_asked; }

void Assign::set_politely_asked() noexcept { _is_politely_asked = true; }

expr *Assign::execute(exec::GlobalCtx &ctx) const {
    try {
        var::assign(exec::execute_rval(ctx, _idx1),
                    exec::execute_rval(ctx, _idx2));
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("[{}] {}", _line, e.what()));
    }
    return nullptr;
}

unsigned Assign::get_line() const noexcept { return _line; }
}  // namespace ast
