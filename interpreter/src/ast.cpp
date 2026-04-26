#include "interpreter/ast.hpp"

namespace ast {

expr *Ast::get_expr(unsigned expr_idx) {
    if (expr_idx >= _exprs.size())
        throw std::runtime_error(
            std::format("Expr_idx: {} is out of range\n", expr_idx));
    return &_exprs[expr_idx];
}

rval &Ast::get_rval(unsigned rval_idx) {
    if (rval_idx >= _rvals.size())
        throw std::runtime_error(
            std::format("Rval_idx: {} is out of range\n", rval_idx));
    return _rvals[rval_idx];
}

void Ast::add_task_metainf(TaskMetainf &&task_metainf) {
    _tasks_metainf.emplace_back(std::move(task_metainf));
}

const Ast::TaskMetainf *Ast::find_task_metainf(
    unsigned task_idx) const noexcept {
    auto it =
        std::find_if(_tasks_metainf.begin(), _tasks_metainf.end(),
                     [task_idx](auto &el) { return el.task_idx == task_idx; });
    if (it == _tasks_metainf.end()) return nullptr;
    return &(*it);
}

const Ast::TaskMetainf *Ast::find_task_metainf(
    std::string_view task_name) const noexcept {
    auto it = std::find_if(
        _tasks_metainf.begin(), _tasks_metainf.end(),
        [task_name](auto &el) { return el.task_name == task_name; });
    if (it == _tasks_metainf.end()) return nullptr;
    return &(*it);
}
}  // namespace ast
