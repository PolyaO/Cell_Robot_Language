#include "backend/rvals/rval.hpp"

namespace ast {
class Extend {
   private:
    unsigned _rval_idx;
    unsigned _dim;

   public:
    Extend(unsigned _rval_idx, unsigned dim);

    Var execute(ExecCtx &ctx) const;
};
}  // namespace ast
