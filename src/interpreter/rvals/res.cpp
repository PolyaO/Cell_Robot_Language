#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/execute.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "var/var.hpp"
#include "var/var_ops.hpp"

namespace ast {
Res::Res(unsigned task_idx, unsigned line) : _task_idx(task_idx), _line(line) {}

var::var_type Res::execute(exec::GlobalCtx &ctx) const {
    auto metainf = *ctx.ast.find_task_metainf(_task_idx);
    if (metainf.task_name == "FINDEXIT") return var::FALSE;
    auto res_idx = metainf.res_ref_idx;
    try {
        return exec::execute_rval(ctx, res_idx);
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("[{}] {}", _line, e.what()));
    }
}
unsigned Res::get_line() const noexcept { return _line; }
}  // namespace ast
