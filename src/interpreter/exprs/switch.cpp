#include <stdexcept>

#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/execute.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/exprs/expr.hpp"
#include "interpreter/rvals/rval.hpp"
#include "var/var_ops.hpp"

namespace ast {
Switch::Switch(unsigned condition_idx, unsigned expr_true, unsigned expr_false,
               unsigned line)
    : _condition_idx(condition_idx),
      _expr_true(expr_true),
      _expr_false(expr_false),
      _line(line) {}

expr *Switch::execute(exec::GlobalCtx &ctx) const {
    try {
        if (var::equal(exec::execute_rval(ctx, _condition_idx), var::TRUE) &&
            _expr_true)
            return ctx.ast.get_expr(_expr_true);
        if (_expr_false) return ctx.ast.get_expr(_expr_false);
        return nullptr;
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(
            std::format("[{}] {}: {}", _line, e.what(),
                        "In switch condition result should be TRUE or FALSE "
                        "(boolean matrix dim [1])"));
    }
}

unsigned Switch::get_line() const noexcept { return _line; }
}  // namespace ast
