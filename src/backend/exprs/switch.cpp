#include <variant>

#include "backend/exprs/expr.hpp"
#include "backend/rvals.hpp"
#include "backend/rvals/rval.hpp"
#include "backend/rvals/var/bool.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Switch::Switch(unsigned condition_idx, unsigned expr_true,
                    unsigned expr_false, unsigned line)
    : _condition_idx(condition_idx),
      _expr_true(expr_true),
      _expr_false(expr_false),
      _line(line) {}

ast::expr *ast::Switch::execute(ExecCtx &ctx) {
    if (_is_executed) {
        _is_executed = false;
        return nullptr;
    }
    _is_executed = true;
    var::var_type v = ast::execute(ctx, ctx.ast.get_rval(_condition_idx));
    if (!std::holds_alternative<var::Var<bool_t>>(v)) {
        throw InterpreterRuntimeError(std::format(
            "{} In switch condition result should be boolean", _line));
    }
    if (var::equal(v, var::TRUE) && _expr_true) {
        return ctx.ast.get_expr(_expr_true);
    }
    if (_expr_false) return ctx.ast.get_expr(_expr_false);
    return nullptr;
}

unsigned ast::Switch::get_line() const noexcept { return _line; }
