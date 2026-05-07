#pragma once
#include <functional>

#include "var/var.hpp"

namespace exec {
class GlobalCtx;
}

namespace ast::rvals {

class Binary {
   public:
    using Op = std::function<var::var_type(const var::var_type &,
                                           const var::var_type &)>;
    Binary(unsigned idx1, unsigned idx2, Op op);
    var::var_type execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _idx1;
    unsigned _idx2;
    Op _op;
};

class Unary {
   public:
    using Op = std::function<var::var_type(const var::var_type &)>;
    Unary(unsigned idx, Op op);
    var::var_type execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _idx;
    Op _op;
};

class Env {
   public:
    var::var_type execute(exec::GlobalCtx &ctx) const;
};

class Res {
   public:
    Res(unsigned task_idx);
    var::var_type execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _task_idx;
};

class Ref {
   public:
    Ref(unsigned idx_in_ctx);
    var::var_type execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _idx_in_ctx;
};

class Idx {
   public:
    Idx(unsigned idx, std::vector<unsigned> &&dim);
    var::var_type execute(exec::GlobalCtx &ctx) const;

   private:
    std::vector<unsigned> _dim;
    unsigned _idx;
};

class Reduce {
   public:
    Reduce(unsigned _idx, unsigned dim, unsigned change);
    var::var_type execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _idx;
    unsigned _dim;
    unsigned _change;
};

class Extend {
   public:
    Extend(unsigned idx, unsigned dim, unsigned change);
    var::var_type execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _idx;
    unsigned _dim;
    unsigned _change;
};
}  // namespace ast::rvals
