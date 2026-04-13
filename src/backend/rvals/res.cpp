#include <stdexcept>

#include "backend/rvals.hpp"
#include "backend/rvals/rval.hpp"
#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Res::Res(unsigned task_idx, unsigned line)
    : _task_idx(task_idx), _line(line) {}

var::var_type ast::Res::execute(ExecCtx &ctx) const {
    auto res_idx = std::get<Task>(*ctx.ast.get_expr(_task_idx)).get_res_idx();
    try {
        return ast::execute(ctx, ctx.ast.get_rval(res_idx));
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("{} {}", _line, e.what()));
    }
}
unsigned ast::Res::get_line() { return _line; }
