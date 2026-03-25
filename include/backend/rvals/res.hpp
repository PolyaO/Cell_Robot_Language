#pragma once

#include "backend/rvals/rval.hpp"

namespace ast {
class Res {
   private:
    unsigned _task_idx;

   public:
    Res(unsigned task_idx);

    Var execute(ExecCtx &ctx) const;
};
}  // namespace ast
