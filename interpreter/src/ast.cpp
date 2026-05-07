#include "interpreter/ast.hpp"

#include "interpreter/expr/expr.hpp"
#include "interpreter/rval/rval.hpp"

namespace ast {

Expr *Ast::get_expr(unsigned expr_idx) noexcept {
    if (expr_idx >= _exprs.size()) return nullptr;
    return &_exprs[expr_idx];
}

Rval *Ast::get_rval(unsigned rval_idx) noexcept {
    if (rval_idx >= _rvals.size()) return nullptr;
    return &_rvals[rval_idx];
}

void Ast::add_task_metainf(TaskMetainf &&task_metainf) {
    _tasks_metainf.emplace_back(std::move(task_metainf));
}

Ast::TaskMetainf *Ast::find_task_metainf(unsigned task_idx) noexcept {
    auto it =
        std::find_if(_tasks_metainf.begin(), _tasks_metainf.end(),
                     [task_idx](auto &el) { return el.task_idx == task_idx; });
    if (it == _tasks_metainf.end()) return nullptr;
    return &(*it);
}

Ast::TaskMetainf *Ast::find_task_metainf(std::string_view task_name) noexcept {
    auto it = std::find_if(
        _tasks_metainf.begin(), _tasks_metainf.end(),
        [task_name](auto &el) { return el.task_name == task_name; });
    if (it == _tasks_metainf.end()) return nullptr;
    return &(*it);
}
}  // namespace ast
