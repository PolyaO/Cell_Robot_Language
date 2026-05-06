#include "interpreter/exec/global_ctx.hpp"
namespace ast {
RotateR::RotateR(unsigned line) : _line(line) {}
bool RotateR::is_politely_asked() const noexcept { return _is_politely_asked; }
void RotateR::set_politely_asked() noexcept { _is_politely_asked = true; }
expr *RotateR::execute(exec::GlobalCtx &ctx) const {
    ctx.robot->ask(_is_politely_asked);
    ctx.robot->rotate_r();
    return nullptr;
}
unsigned RotateR::get_line() const noexcept { return _line; }
}  // namespace ast
