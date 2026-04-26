#include "interpreter/exprs/task.hpp"

#include "interpreter/exec/global_ctx.hpp"

namespace ast {
Task::Task(std::vector<unsigned> &&exprs, unsigned counter_idx, unsigned line)
    : _exprs(exprs), _counter_idx(counter_idx), _line(line) {}

unsigned Task::get_line() const noexcept { return _line; }

expr *Task::execute(exec::GlobalCtx &ctx) const {
    auto &curr_ctx = ctx.get_curr_ctx();
    auto next_expr_idx = curr_ctx.get_counter(_counter_idx);
    if (next_expr_idx == _exprs.size()) {
        curr_ctx.set_counter(0, _counter_idx);
        ctx.pop_curr_ctx();
        return nullptr;
    }
    curr_ctx.increment_counter(_counter_idx);
    return ctx.ast.get_expr(_exprs[next_expr_idx]);
}
}  // namespace ast

