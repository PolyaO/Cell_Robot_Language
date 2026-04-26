#include <iostream>

#include "interpreter/exec/global_ctx.hpp"
namespace ast {
RotateL::RotateL(unsigned line) : _line(line) {}
bool RotateL::is_politely_asked() const noexcept { return _is_politely_asked; }
void RotateL::set_politely_asked() noexcept { _is_politely_asked = true; }
expr *RotateL::execute(exec::GlobalCtx &ctx) const {
    std::cout << std::format("[{}] ROBOT ROTATE_LEFT! politeness : {}", _line,
                             _is_politely_asked)
              << std::endl;
    return nullptr;
}
unsigned RotateL::get_line() const noexcept { return _line; }
}  // namespace ast
