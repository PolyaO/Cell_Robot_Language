#include <iostream>

#include "interpreter/exec_ctx.hpp"

ast::Move::Move(unsigned line) : _line(line) {}
bool ast::Move::is_politely_asked() const noexcept {
    return _is_politely_asked;
}
void ast::Move::set_politely_asked() noexcept { _is_politely_asked = true; }
ast::expr *ast::Move::execute(ExecCtx &ctx) {
    std::cout << std::format("[{}] ROBOT MOVE! politeness : {}", _line,
                             _is_politely_asked)
              << std::endl;
    return nullptr;
}
unsigned ast::Move::get_line() const noexcept { return _line; }
