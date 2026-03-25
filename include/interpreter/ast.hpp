#pragma once
#include <backend/exprs.hpp>
#include <backend/rvals.hpp>
#include <concepts>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include "interpreter/defs.hpp"

namespace ast {
class Ast {
   public:
    using metainf_vector_t =
        std::vector<std::tuple<std::string, unsigned, variables_t>>;

    Ast();
    Ast &operator=(Ast &&other) noexcept = default;

    expr *get_expr(unsigned expr_idx);
    expr *get_find_exit();
    rval &get_rval(unsigned rval_idx);
    void add_task_metainf(std::string_view task_name, unsigned task_idx,
                          variables_t &&map);
    unsigned get_expr_idx(expr *_expr);

    std::optional<unsigned> get_task_idx(std::string_view task_name);
    std::string_view get_task_name(unsigned task_idx);
    std::optional<Var> get_variable(std::string_view var_name,
                                    unsigned task_idx);

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

   private:
    std::vector<expr> _exprs;
    std::vector<rval> _rvals;
    metainf_vector_t _tasks_metainf;

    metainf_vector_t::const_iterator find_task_metainf_by_idx(
        unsigned task_idx) noexcept;
    metainf_vector_t::const_iterator find_task_metainf_by_name(
        std::string_view task_name) noexcept;
};
}  // namespace ast
