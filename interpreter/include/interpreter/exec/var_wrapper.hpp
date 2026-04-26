#pragma once

#include <variant>

#include "var/var.hpp"
namespace exec {
class VarWrapper {
   public:
    var::var_type *get();
    void set(var::var_type &&v);
    void set(var::var_type *v);

   private:
    std::variant<var::var_type *, var::var_type> _var;
};
}  // namespace exec
