#include <variant>

#include "interpreter/exec/global_ctx.hpp"
#include "var/var.hpp"

namespace ast {
Declare::Declare(unsigned idx_in_ctx, const var::dim_t &dim,
                 std::variant<int, bool_t> &&val, unsigned line)
    : _val(val), _dim_size(dim.size()), _idx_in_ctx(idx_in_ctx), _line(line) {
    _dim = std::make_unique<unsigned[]>(_dim_size);
    std::copy(dim.begin(), dim.end(), _dim.get());
}

bool Declare::is_politely_asked() const noexcept { return _is_politely_asked; }

void Declare::set_politely_asked() noexcept { _is_politely_asked = true; }

expr *Declare::execute(exec::GlobalCtx &ctx) const {
    auto &curr_ctx = ctx.get_curr_ctx();
    if (std::holds_alternative<bool_t>(_val)) {
        curr_ctx.set_var(var::var_type(var::Var<bool_t>(
                             std::get<bool_t>(_val),
                             std::span<const unsigned>(_dim.get(), _dim_size))),
                         _idx_in_ctx);
    } else {
        curr_ctx.set_var(var::var_type(var::Var<int>(
                             std::get<int>(_val),
                             std::span<const unsigned>(_dim.get(), _dim_size))),
                         _idx_in_ctx);
    }
    return nullptr;
}

unsigned Declare::get_line() const noexcept { return _line; }

}  // namespace ast
