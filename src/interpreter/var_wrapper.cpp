#include "interpreter/exec/var_wrapper.hpp"

#include <variant>

#include "var/var.hpp"

namespace exec {
var::var_type *VarWrapper::get() {
    if (std::holds_alternative<var::var_type>(_var))
        return &std::get<var::var_type>(_var);
    return std::get<var::var_type *>(_var);
}

void VarWrapper::set(var::var_type &&v) { _var = v; }

void VarWrapper::set(var::var_type *v) { _var = v; }
}  // namespace exec
