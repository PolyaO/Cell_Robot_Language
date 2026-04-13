#include "backend/exprs/assign.hpp"

#include <backend/exprs.hpp>
#include <stdexcept>

#include "backend/rvals.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include "interpreter/exceptions/build_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Assign::Assign(unsigned idx1, unsigned idx2, unsigned line)
    : _idx1(idx1), _idx2(idx2), _line(line) {}
bool ast::Assign::is_politely_asked() const noexcept {
    return _is_politely_asked;
}
void ast::Assign::set_politely_asked() noexcept { _is_politely_asked = true; }
ast::expr *ast::Assign::execute(ExecCtx &ctx) {
    try {
        var::assign(ast::execute(ctx, ctx.ast.get_rval(_idx1)),
                    ast::execute(ctx, ctx.ast.get_rval(_idx2)));
    } catch (const std::runtime_error &e) {
        throw InterpreterBuildError(std::format("{} {}", _line, e.what()));
    }
    return nullptr;
}
unsigned ast::Assign::get_line() const noexcept { return _line; }
