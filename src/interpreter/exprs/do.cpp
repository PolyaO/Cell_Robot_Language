#include "interpreter/exprs/do.hpp"

#include <ranges>

#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/exec/local_ctx.hpp"
#include "var/var.hpp"
namespace ast {
Do::Do(unsigned task_idx, std::vector<unsigned> &&arg_list, unsigned line)
    : _args_num(arg_list.size()), _task_idx(task_idx), _line(line) {
    _arg_list = std::make_unique<unsigned[]>(_args_num);
    std::copy(arg_list.begin(), arg_list.end(), _arg_list.get());
}

bool Do::is_politely_asked() const noexcept { return _is_politely_asked; }
void Do::set_politely_asked() noexcept { _is_politely_asked = true; }

void Do::set_task_idx(unsigned idx) noexcept { _task_idx = idx; }

expr *Do::execute(exec::GlobalCtx &ctx) const {
    const auto &task_info = *ctx.ast.find_task_metainf(_task_idx);
    throw_if_wrong_args_num(task_info.args_number);
    exec::LocalCtx new_ctx(task_info.ctx_vars_number,
                           task_info.ctx_scope_counters_number);
    new_ctx.set_task_idx(_task_idx);
    auto &curr_ctx = ctx.get_curr_ctx();
    var::var_type *var_ptr;

    for (auto &&[new_idx, curr_idx] :
         std::span<unsigned>(_arg_list.get(), _args_num) |
             std::views::enumerate) {
        var_ptr = curr_ctx.get_var(curr_idx);
        new_ctx.set_var(var_ptr, new_idx);
    }
    ctx.push_curr_ctx(std::move(new_ctx));
    return ctx.ast.get_expr(_task_idx);
}

unsigned Do::get_line() const noexcept { return _line; }

void Do::throw_if_wrong_args_num(unsigned expected) const {
    if (_args_num != expected)
        throw InterpreterRuntimeError(
            std::format("[{}] In task call wrong number of arguments passed. "
                        "number: {}; expected: {};",
                        _line, _args_num, expected));
}
}  // namespace ast

