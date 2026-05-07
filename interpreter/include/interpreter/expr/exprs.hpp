#pragma once
#include <memory>
#include <variant>

#include "var/bool.hpp"
#include "var/var.hpp"

namespace exec {
class GlobalCtx;
}
namespace ast {
class Expr;
}
namespace ast::exprs {
class Declare {
   public:
    Declare(unsigned idx_in_ctx, const var::dim_t &dim,
            std::variant<int, bool_t> &&val);
    Expr *execute(exec::GlobalCtx &ctx) const;

   private:
    std::unique_ptr<unsigned[]> _dim;
    std::variant<int, bool_t> _val;
    unsigned _dim_size;
    unsigned _idx_in_ctx;
};

class Assign {
   public:
    Assign(unsigned idx1, unsigned idx2);
    Expr *execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _idx1;
    unsigned _idx2;
};

class AssignVal {
   public:
    AssignVal(unsigned idx, std::variant<int, bool_t> &&val);
    Expr *execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _idx;
    std::variant<int, bool_t> _val;
};

class Digitize {
   public:
    Digitize(unsigned idx);
    Expr *execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _idx;
};

class Logitize {
   public:
    Logitize(unsigned idx);
    Expr *execute(exec::GlobalCtx &ctx) const;
    unsigned get_line() const noexcept;

   private:
    unsigned _idx;
};

class Do {
   public:
    Do(unsigned task_idx, unsigned is_executed_idx,
       std::vector<unsigned> &&arg_list);
    void set_task_idx(unsigned idx) noexcept;
    Expr *execute(exec::GlobalCtx &ctx) const;

   private:
    void throw_if_wrong_args_num(unsigned expected) const;

    std::unique_ptr<unsigned[]> _arg_list;
    unsigned _args_num;
    unsigned _task_idx;
    unsigned _is_executed_idx;
    unsigned _line;
};

class Scope {
   public:
    Scope(std::vector<unsigned> &&exprs, unsigned counter_idx);
    Expr *execute(exec::GlobalCtx &ctx) const;

   private:
    std::vector<unsigned> _exprs;
    unsigned _counter_idx;
};

class For {
   public:
    For(unsigned counter_idx, unsigned boundary_idx, unsigned step_idx,
        unsigned stmt_idx, unsigned reserve_counter_idx,
        unsigned dim_number_idx, unsigned is_first_step_idx);
    Expr *execute(exec::GlobalCtx &ctx) const;

   private:
    void throw_if_diff_dims(var::Var<int> &a, var::Var<int> &b,
                            var::Var<int> &c) const;
    Expr *first_step_case(exec::GlobalCtx &ctx) const;
    Expr *last_step_case(exec::GlobalCtx &ctx) const;

    unsigned _counter_idx;
    unsigned _boundary_idx;
    unsigned _step_idx;
    unsigned _stmt_idx;

    unsigned _reserve_counter_idx;
    unsigned _dim_number_idx;
    unsigned _is_first_step_idx;
};

class Switch {
   public:
    Switch(unsigned condition_idx, unsigned is_executed_idx, unsigned expr_true,
           unsigned expr_false);
    Expr *execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _expr_true = -1;
    unsigned _expr_false = -1;
    unsigned _condition_idx;
    unsigned _is_executed_idx;
};

class RetRes {
   public:
    RetRes(unsigned idx_in_ctx);
    Expr *execute(exec::GlobalCtx &ctx) const;

   private:
    unsigned _idx_in_ctx;
};

class Move {
   public:
    Move() = default;
    Expr *execute(exec::GlobalCtx &ctx) const;
};

class RotateR {
   public:
    RotateR() = default;
    Expr *execute(exec::GlobalCtx &ctx) const;
};

class RotateL {
   public:
    RotateL() = default;
    Expr *execute(exec::GlobalCtx &ctx) const;
};
}  // namespace ast::exprs
