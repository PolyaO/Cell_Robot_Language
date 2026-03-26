#pragma once

#include "backend/rvals/rval.hpp"

namespace ast {
class Res {
   public:
    Res(unsigned task_idx);

    var::var_type execute(ExecCtx &ctx) const;
    unsigned get_line();

   private:
    unsigned _task_idx;
    unsigned _line;
};
}  // namespace ast
