#include <variant>

#include "interpreter/exceptions/runtime_exceptions.hpp"
#include "interpreter/exec/execute.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "var/bool.hpp"
#include "var/var_ops.hpp"

namespace ast {
AssignVal::AssignVal(unsigned idx, std::variant<int, bool_t> &&val,
                     unsigned line)
    : _idx(idx), _val(val), _line(line) {}

void AssignVal::set_politely_asked() noexcept { _is_politely_asked = true; }

bool AssignVal::is_politely_asked() const noexcept {
    return _is_politely_asked;
}

expr *AssignVal::execute(exec::GlobalCtx &ctx) const {
    try {
        if (std::holds_alternative<int>(_val))
            var::assign_val<int>(exec::execute_rval(ctx, _idx),
                                 std::get<int>(_val));
        else
            var::assign_val<bool_t>(exec::execute_rval(ctx, _idx),
                                    std::get<bool_t>(_val));
    } catch (const std::runtime_error &e) {
        throw InterpreterRuntimeError(std::format("[{}] {}", _line, e.what()));
    }
    return nullptr;
}

unsigned AssignVal::get_line() const noexcept { return _line; }

}  // namespace ast
