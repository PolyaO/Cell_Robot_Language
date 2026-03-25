#pragma once

#include "backend/rvals/rval.hpp"
namespace ast {
class Env {
   public:
    Var execute(ExecCtx &ctx) const;
};
}  // namespace ast
