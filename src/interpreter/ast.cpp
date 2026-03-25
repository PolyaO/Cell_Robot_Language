#include "interpreter/ast.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <stdexcept>
#include <string_view>

#include "interpreter/exceptions/runtime_exceptions.hpp"

ast::Ast::Ast() {
    make_rval<Var>(false, std::vector<unsigned>());
}
ast::expr *ast::Ast::get_expr(unsigned expr_idx) {
    if (expr_idx >= _exprs.size())
        throw std::runtime_error(
            std::format("Expr_idx: {} is out of range\n", expr_idx));
    return &_exprs[expr_idx];
}

ast::expr *ast::Ast::get_find_exit() {
    auto it = find_task_metainf_by_name("FINDEXIT");
    if (it == _tasks_metainf.end())
        throw InterpreterExecError("No task named FIND_EXIT found\n");
    return get_expr(std::get<1>(*it));
}

ast::rval &ast::Ast::get_rval(unsigned rval_idx) {
    if (rval_idx >= _rvals.size())
        throw std::runtime_error(
            std::format("Rval_idx: {} is out of range\n", rval_idx));
    return _rvals[rval_idx];
}

void ast::Ast::add_task_metainf(std::string_view task_name, unsigned task_idx,
                                variables_t &&map) {
    _tasks_metainf.emplace_back(task_name, task_idx, std::move(map));
}

unsigned ast::Ast::get_expr_idx(ast::expr *_expr) {
    unsigned res = _expr - _exprs.data();
    if (res >= _exprs.size())
        throw std::runtime_error(
            "Cannot get expr_idx. _exprs does not contain given expr\n");
    return res;
}

std::optional<unsigned> ast::Ast::get_task_idx(std::string_view task_name) {
    auto it = find_task_metainf_by_name(task_name);
    if (it == _tasks_metainf.end()) return std::optional<unsigned>();
    return std::get<1>(*it);
}

std::string_view ast::Ast::get_task_name(unsigned task_idx) {
    auto it = find_task_metainf_by_idx(task_idx);
    if (it == _tasks_metainf.end())
        throw std::runtime_error(std::format(
            "Cannot get task_name. No task with idx {}\n", task_idx));
    return std::get<0>(*it);
}

std::optional<ast::Var> ast::Ast::get_variable(std::string_view var_name,
                                               unsigned task_idx) {
    auto it = find_task_metainf_by_idx(task_idx);
    if (it == _tasks_metainf.end())
        throw std::runtime_error(std::format(
            "Cannot get variable. No task with idx {}\n", task_idx));
    auto metainf = std::get<2>(*it);
    std::string key(var_name);
    if (!metainf.contains(key)) return std::optional<ast::Var>();
    return std::get<Var>(_rvals[metainf[key]]);
}

ast::Ast::metainf_vector_t::const_iterator ast::Ast::find_task_metainf_by_idx(
    unsigned task_idx) noexcept {
    return std::find_if(
        _tasks_metainf.begin(), _tasks_metainf.end(),
        [task_idx](auto &el) { return std::get<1>(el) == task_idx; });
}

ast::Ast::metainf_vector_t::const_iterator ast::Ast::find_task_metainf_by_name(
    std::string_view task_name) noexcept {
    return std::find_if(
        _tasks_metainf.begin(), _tasks_metainf.end(),
        [task_name](auto &el) { return std::get<0>(el) == task_name; });
}

