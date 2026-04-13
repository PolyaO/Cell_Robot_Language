#include "backend/exprs/for.hpp"

#include <ranges>

#include "backend/rvals/rval.hpp"
#include "backend/rvals/var/var.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::For::For(unsigned counter, unsigned curr_counter, unsigned boundary,
              unsigned step, unsigned stmt, unsigned line)
    : _counter(counter),
      _curr_counter(curr_counter),
      _boundary(boundary),
      _step(step),
      _stmt(stmt),
      _line(line) {}

ast::expr *ast::For::execute(ExecCtx &ctx) {
    var::Var<int> &counter = std::get<var::Var<int>>(
        std::get<var::var_type>(ctx.ast.get_rval(_curr_counter)));
    var::Var<int> &step = std::get<var::Var<int>>(
        std::get<var::var_type>(ctx.ast.get_rval(_step)));
    var::Var<int> &boundary = std::get<var::Var<int>>(
        std::get<var::var_type>(ctx.ast.get_rval(_boundary)));
    check_for_same_dims(counter, step, boundary);

    if (_is_first_step) {
        var::assign(std::get<var::var_type>(ctx.ast.get_rval(_curr_counter)),
                    std::get<var::var_type>(ctx.ast.get_rval(_counter)));
        _is_first_step = false;
        return ctx.ast.get_expr(_stmt);
    }

    for (auto &&[cnt, stp, bnd] : std::views::zip(counter, step, boundary)) {
        if (stp < 0) {
            if (cnt > bnd) {
                cnt += stp;
                return ctx.ast.get_expr(_stmt);
            }
        } else {
            if (cnt < bnd) {
                cnt -= stp;
                return ctx.ast.get_expr(_stmt);
            }
        }
    }
    _is_first_step = true;
    return nullptr;
}

unsigned ast::For::get_line() const noexcept { return _line; }
void ast::For::check_for_same_dims(var::Var<int> &a, var::Var<int> &b,
                                   var::Var<int> &c) {
    if (a.get_dim() != b.get_dim() || a.get_dim() != c.get_dim())
        throw InterpreterRuntimeError(
            std::format("{} Variables with different dimensions used in for "
                        "expr as step counter and boundary",
                        _line));
}
