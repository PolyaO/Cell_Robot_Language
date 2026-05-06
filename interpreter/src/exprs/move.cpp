#include <iostream>

#include "interpreter/exec/global_ctx.hpp"
namespace ast {
Move::Move(unsigned line) : _line(line) {}
bool Move::is_politely_asked() const noexcept { return _is_politely_asked; }
void Move::set_politely_asked() noexcept { _is_politely_asked = true; }
expr *Move::execute(exec::GlobalCtx &ctx) const {
    ctx.robot->ask(_is_politely_asked);
    ctx.robot->move();
    return nullptr;
}
unsigned Move::get_line() const noexcept { return _line; }
}  // namespace ast
