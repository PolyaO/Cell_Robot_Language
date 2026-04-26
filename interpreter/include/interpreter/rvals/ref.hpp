#pragma once

#include "interpreter/rvals/rval.hpp"
#include "var/var.hpp"
namespace ast {
class Ref {
   public:
    Ref(unsigned idx_in_ctx, unsigned line);
    var::var_type execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _idx_in_ctx;
    unsigned _line;
};
}  // namespace ast
