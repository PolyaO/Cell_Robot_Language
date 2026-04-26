#include "interpreter/exprs/for.hpp"

#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "var/var.hpp"
#include "var/var_ops.hpp"
namespace ast {
For::For(unsigned counter_idx, unsigned boundary_idx, unsigned step_idx,
         unsigned stmt_idx, unsigned reserve_counter_idx,
         unsigned dim_number_idx, unsigned is_first_step_idx, unsigned line)
    : _counter_idx(counter_idx),
      _boundary_idx(boundary_idx),
      _step_idx(step_idx),
      _stmt_idx(stmt_idx),
      _reserve_counter_idx(reserve_counter_idx),
      _dim_number_idx(dim_number_idx),
      _is_first_step_idx(is_first_step_idx),
      _line(line) {}

unsigned For::get_line() const noexcept { return _line; }

expr *For::execute(exec::GlobalCtx &ctx) const {
    auto &curr_ctx = ctx.get_curr_ctx();

    var::Var<int> &counter =
        std::get<var::Var<int>>(*curr_ctx.get_var(_counter_idx));
    var::Var<int> &step = std::get<var::Var<int>>(*curr_ctx.get_var(_step_idx));
    var::Var<int> &boundary =
        std::get<var::Var<int>>(*curr_ctx.get_var(_boundary_idx));

    throw_if_diff_dims(counter, step, boundary);

    expr *res = first_step_case(ctx);
    if (res) return res;

    unsigned dim_number = curr_ctx.get_counter(_dim_number_idx);
    int &cnt = counter[dim_number];
    int &stp = step[dim_number];
    int &bnd = boundary[dim_number];
    cnt += stp;
    if (stp > 0 && cnt < bnd) return ctx.ast.get_expr(_stmt_idx);
    if (stp < 0 && cnt > bnd) return ctx.ast.get_expr(_stmt_idx);
    curr_ctx.increment_counter(_dim_number_idx);
    if (last_step_case(ctx)) return nullptr;
    return ctx.ast.get_expr(_stmt_idx);
}

void For::throw_if_diff_dims(var::Var<int> &a, var::Var<int> &b,
                             var::Var<int> &c) const {
    if (a.get_dim() != b.get_dim() || a.get_dim() != c.get_dim())
        throw InterpreterRuntimeError(
            std::format("[{}] Variables with different dimensions used in for "
                        "expr as step counter and boundary",
                        _line));
}

expr *For::last_step_case(exec::GlobalCtx &ctx) const {
    auto &curr_ctx = ctx.get_curr_ctx();
    var::Var<int> counter =
        std::get<var::Var<int>>(*curr_ctx.get_var(_counter_idx));
    unsigned size = counter.end() - counter.begin();
    unsigned dim_number = curr_ctx.get_counter(_dim_number_idx);

    if (dim_number == size) {
        curr_ctx.set_counter(0, _dim_number_idx);
        curr_ctx.set_counter(0, _is_first_step_idx);
        var::assign(*curr_ctx.get_var(_counter_idx),
                    *curr_ctx.get_var(_reserve_counter_idx));
        curr_ctx.set_var(nullptr, _reserve_counter_idx);
        return (expr *)1;
    }
    return nullptr;
}
expr *For::first_step_case(exec::GlobalCtx &ctx) const {
    auto &curr_ctx = ctx.get_curr_ctx();
    if (!curr_ctx.get_var(_reserve_counter_idx)) {
        std::vector<unsigned> dim =
            var::get_dim(*curr_ctx.get_var(_counter_idx));
        curr_ctx.set_var(var::var_type(var::Var<int>(0, dim)),
                         _reserve_counter_idx);
        curr_ctx.set_counter(1, _is_first_step_idx);
        var::assign(*curr_ctx.get_var(_reserve_counter_idx),
                    *curr_ctx.get_var(_counter_idx));
        return ctx.ast.get_expr(_stmt_idx);
    }
    if (!curr_ctx.get_counter(_is_first_step_idx)) {
        curr_ctx.set_counter(1, _is_first_step_idx);
        return ctx.ast.get_expr(_stmt_idx);
    }
    return nullptr;
}

}  // namespace ast
