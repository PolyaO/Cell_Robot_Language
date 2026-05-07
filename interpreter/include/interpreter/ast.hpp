#pragma once
#include <concepts>
#include <interpreter/expr/exprs.hpp>
#include <interpreter/rval/rval.hpp>
#include <interpreter/rval/rvals.hpp>
#include <string_view>
#include <utility>
#include <vector>

#include "interpreter/expr/expr.hpp"
#include "var/var.hpp"
#include "var/var_ops.hpp"

namespace ast {
class Ast {
   public:
    struct VarMetainf {
        std::string var_name;
        unsigned ref_idx;
        unsigned real_idx;
    };

    using vars_metainf_t = std::vector<VarMetainf>;

    struct TaskMetainf {
        std::string task_name;
        var::var_type res = var::FALSE;
        vars_metainf_t vars_metainf;
        unsigned decl_lineno;
        unsigned task_idx;
        unsigned args_number;
        unsigned ctx_vars_number;
        unsigned ctx_counters_number;
    };

    using tasks_metainf_t = std::vector<TaskMetainf>;

    Ast() = default;

    Expr *get_expr(unsigned expr_idx) noexcept;
    Rval *get_rval(unsigned rval_idx) noexcept;

    void add_task_metainf(TaskMetainf &&task_metainf);

    template <ExprType T, class... Args>
        requires(std::constructible_from<T, Args...>)
    unsigned make_expr(unsigned lineno, bool is_scopelike_expr,
                       Args &&...args) {
        _exprs.emplace_back(lineno, is_scopelike_expr,
                            T(std::forward<Args>(args)...));
        return _exprs.size() - 1;
    }

    template <RvalType T, class... Args>
        requires(std::constructible_from<T, Args...>)
    unsigned make_rval(unsigned lineno, Args &&...args) {
        _rvals.emplace_back(lineno, T(std::forward<Args>(args)...));
        return _rvals.size() - 1;
    }

    TaskMetainf *find_task_metainf(unsigned task_idx) noexcept;
    TaskMetainf *find_task_metainf(
        std::string_view task_name) noexcept;

   private:
    std::vector<Expr> _exprs;
    std::vector<Rval> _rvals;
    tasks_metainf_t _tasks_metainf;
};
}  // namespace ast
