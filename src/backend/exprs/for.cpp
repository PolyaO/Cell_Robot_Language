#include "backend/exprs/for.hpp"

#include <ranges>

#include "backend/rvals.hpp"
#include "backend/rvals/rval.hpp"
#include "backend/rvals/var/var.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::For::For(unsigned counter, unsigned boundary, unsigned step, unsigned stmt,
              unsigned line)
    : _counter(counter),
      _reserve_counter(0),
      _boundary(boundary),
      _step(step),
      _stmt(stmt),
      _line(line) {}

ast::expr *ast::For::execute(ExecCtx &ctx) {
    var::Var<int> counter =
        std::get<var::Var<int>>(ast::execute(ctx, ctx.ast.get_rval(_counter)));
    var::Var<int> step =
        std::get<var::Var<int>>(ast::execute(ctx, ctx.ast.get_rval(_step)));
    var::Var<int> boundary =
        std::get<var::Var<int>>(ast::execute(ctx, ctx.ast.get_rval(_boundary)));

    check_for_same_dims(counter, step, boundary);
    if (!_reserve_counter) {
        std::vector<unsigned> counter_dim =
            var::get_dim(ast::execute(ctx, ctx.ast.get_rval(_counter)));
        _reserve_counter =
            ctx.ast.make_rval<var::var_type>(var::Var<int>(0, counter_dim));
        var::assign(ast::execute(ctx, ctx.ast.get_rval(_reserve_counter)),
                    ast::execute(ctx, ctx.ast.get_rval(_counter)));
    }
    if (_is_first_step) {
        var::assign(ast::execute(ctx, ctx.ast.get_rval(_counter)),
                    ast::execute(ctx, ctx.ast.get_rval(_reserve_counter)));
        _is_first_step = false;
        return ctx.ast.get_expr(_stmt);
    }
    

   // for (auto &&[cnt, stp, bnd] : std::views::zip(counter, step, boundary)) {
   //     cnt += stp;
   //     if (stp < 0) {
   //         if (cnt > bnd) {
   //             return ctx.ast.get_expr(_stmt);
   //         }
   //     } else {
   //         if (cnt < bnd) {
   //             return ctx.ast.get_expr(_stmt);
   //         }
   //     }
   // }
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
