#include "backend/exprs/do.hpp"

#include <ranges>

#include "backend/exprs/task.hpp"
#include "interpreter/defs.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Do::Do(unsigned task_idx, std::vector<unsigned> &&arg_list, unsigned line)
    : _args_num(arg_list.size()), _task_idx(task_idx), _line(line) {
    if (_args_num > 0) {
        _arg_list = std::make_unique<unsigned[]>(_args_num);
        std::copy(arg_list.begin(), arg_list.end(), _arg_list.get());
    }
}

bool ast::Do::is_politely_asked() const noexcept { return _is_politely_asked; }
void ast::Do::set_politely_asked() noexcept { _is_politely_asked = true; }
ast::expr *ast::Do::execute(ExecCtx &ctx) {
    if (_is_executed) {
        _is_executed = false;
        return nullptr;
    }
    _is_executed = true;
    ctx.curr_task_idx = _task_idx;
    expr *task_ptr = ctx.ast.get_expr(_task_idx);
    set_args(ctx);
    _is_executed = true;
    return task_ptr;
}
unsigned ast::Do::get_line() const noexcept { return _line; }

void ast::Do::set_args(ExecCtx &ctx) const {
    check_for_correct_args_num(ctx);
    auto task_info = ctx.ast.find_task_metainf_by_idx(_task_idx);
    const variables_t &variables = std::get<2>(*task_info);
    for (auto &&[arg_idx, v] :
         std::views::zip(std::span<unsigned>(_arg_list.get(), _args_num),
                         std::views::take(variables, _args_num))) {
        unsigned ref_idx = std::get<1>(v);
        Ref &ref = std::get<Ref>(ctx.ast.get_rval(ref_idx));
        ref.set_idx(arg_idx);
    }
}

void ast::Do::check_for_correct_args_num(ExecCtx &ctx) const {
    auto &task = std::get<Task>(*ctx.ast.get_expr(_task_idx));
    auto expected = task.get_args_number();
    if (_args_num != expected)
        throw InterpreterRuntimeError(
            std::format("{} In task call wrong number of arguments passed. "
                        "number: {}; expected: {};",
                        _line, _args_num, expected));
}

