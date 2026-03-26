#pragma once

#include "backend/rvals/rval.hpp"
namespace ast {
class Env {
   public:
    var::var_type execute(ExecCtx &ctx) const;
    unsigned get_line();

   private:
    unsigned _line;
};
}  // namespace ast
