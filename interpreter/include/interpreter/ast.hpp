#pragma once
#include <concepts>
#include <interpreter/exprs.hpp>
#include <interpreter/rvals.hpp>
#include <interpreter/rvals/rval.hpp>
#include <string_view>
#include <utility>
#include <vector>

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
        vars_metainf_t vars_metainf;
        unsigned decl_line;
        unsigned task_idx;
        unsigned args_number;
        unsigned res_ref_idx;
        unsigned ctx_vars_number;
        unsigned ctx_scope_counters_number;
    };
    using tasks_metainf_t = std::vector<TaskMetainf>;

    Ast() = default;

    expr *get_expr(unsigned expr_idx);
    rval &get_rval(unsigned rval_idx);
    void add_task_metainf(TaskMetainf &&task_metainf);

    template <ExprType T, class... Args>
        requires(std::constructible_from<T, Args...>)
    unsigned make_expr(Args &&...args) {
        _exprs.emplace_back(T(std::forward<Args>(args)...));
        return _exprs.size() - 1;
    }

    template <RvalType T, class... Args>
        requires(std::constructible_from<T, Args...>)
    unsigned make_rval(Args &&...args) {
        _rvals.emplace_back(T(std::forward<Args>(args)...));
        return _rvals.size() - 1;
    }

    const TaskMetainf *find_task_metainf(unsigned task_idx) const noexcept;
    const TaskMetainf *find_task_metainf(
        std::string_view task_name) const noexcept;

   private:
    std::vector<expr> _exprs;
    std::vector<rval> _rvals;
    tasks_metainf_t _tasks_metainf;
};
}  // namespace ast
