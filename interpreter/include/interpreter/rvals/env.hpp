#pragma once

#include "interpreter/rvals/rval.hpp"
#include "var/var.hpp"
namespace ast {
class Env {
   public:
    Env(unsigned line);
    var::var_type execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _line;
};
}  // namespace ast
