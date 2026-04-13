#include "backend/exprs/task.hpp"

#include "backend/exprs.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Task::Task(std::vector<unsigned> &&exprs, unsigned res_idx,
                unsigned args_number, unsigned line)
    : _exprs_size(exprs.size()),
      _res_idx(res_idx),
      _args_num(args_number),
      _line(line) {
    _exprs = std::make_unique<unsigned[]>(_exprs_size);
    std::copy(exprs.begin(), exprs.end(), _exprs.get());
}

unsigned ast::Task::get_res_idx() const noexcept { return _res_idx; }
unsigned ast::Task::get_args_number() const noexcept { return _args_num; }
unsigned ast::Task::get_line() const noexcept { return _line; }

ast::expr* ast::Task::execute(ExecCtx &ctx) {
    if (_next_expr_idx == _exprs_size) {
        _next_expr_idx = 0;
        return nullptr;
    }
    return ctx.ast.get_expr(_exprs[_next_expr_idx++]);
}

