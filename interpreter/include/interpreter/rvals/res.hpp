#pragma once

#include "interpreter/rvals/rval.hpp"
#include "var/var.hpp"

namespace ast {
class Res {
   public:
    Res(unsigned task_idx, unsigned line);

    var::var_type execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _task_idx;
    unsigned _line;
};
}  // namespace ast
