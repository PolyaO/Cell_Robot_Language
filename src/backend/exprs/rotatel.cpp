#include <iostream>

#include "interpreter/exec_ctx.hpp"

ast::RotateL::RotateL(unsigned line) : _line(line) {}
bool ast::RotateL::is_politely_asked() const noexcept {
    return _is_politely_asked;
}
void ast::RotateL::set_politely_asked() noexcept { _is_politely_asked = true; }
ast::expr *ast::RotateL::execute(ExecCtx &ctx) {
    std::cout << std::format("[{}] ROBOT ROTATE_LEFT! politeness : {}", _line,
                             _is_politely_asked)
              << std::endl;
    return nullptr;
}
unsigned ast::RotateL::get_line() const noexcept { return _line; }
