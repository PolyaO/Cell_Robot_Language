#include <stdexcept>

#include "backend/rvals.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include "interpreter/exceptions/build_exceptions.hpp"
#include "interpreter/exec_ctx.hpp"

ast::Digitize::Digitize(unsigned idx, unsigned line) : _idx(idx), _line(line) {}
bool ast::Digitize::is_politely_asked() const noexcept {
    return _is_politely_asked;
}
void ast::Digitize::set_politely_asked() noexcept { _is_politely_asked = true; }
ast::expr *ast::Digitize::execute(ExecCtx &ctx) {
    try {
        var::digitize(ast::get_var(ctx, ctx.ast.get_rval(_idx)));
    } catch (const std::runtime_error &e) {
        throw InterpreterBuildError(std::format("{} {}", _line, e.what()));
    }
    return nullptr;
}
unsigned ast::Digitize::get_line() const noexcept { return _line; }
