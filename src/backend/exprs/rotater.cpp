#include <iostream>

#include "interpreter/exec_ctx.hpp"

ast::RotateR::RotateR(unsigned line) : _line(line) {}
bool ast::RotateR::is_politely_asked() const noexcept {
    return _is_politely_asked;
}
void ast::RotateR::set_politely_asked() noexcept { _is_politely_asked = true; }
ast::expr *ast::RotateR::execute(ExecCtx &ctx) {
    std::cout << std::format("[{}] ROBOT ROTATE_RIGHT! politeness : {}", _line,
                             _is_politely_asked)
              << std::endl;
    return nullptr;
}
unsigned ast::RotateR::get_line() const noexcept { return _line; }
