#include "backend/exprs.hpp"
#include "backend/rvals/var/bool.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include "interpreter/exec_ctx.hpp"
#include "interpreter/exceptions/build_exceptions.hpp"
#include <variant>

namespace ast {
AssignVal::AssignVal(unsigned idx, std::variant<int, bool_t> val, unsigned line)
    : _idx(idx), _val(val), _line(line) {}

void AssignVal::set_politely_asked() noexcept { _is_politely_asked = true; }

bool AssignVal::is_politely_asked() const noexcept {
    return _is_politely_asked;
}

unsigned AssignVal::get_line() const noexcept { return _line; }

ast::expr *AssignVal::execute(ExecCtx &ctx) {
    try {
        if (std::holds_alternative<int>(_val))
            var::assign_val<int>(ast::execute(ctx, ctx.ast.get_rval(_idx)),
                                 std::get<int>(_val));
        else
            var::assign_val<bool_t>(ast::execute(ctx, ctx.ast.get_rval(_idx)),
                                    std::get<bool_t>(_val));
    } catch (const std::runtime_error &e) {
        throw InterpreterBuildError(std::format("{} {}", _line, e.what()));
    }
    return nullptr;
}

}  // namespace ast
