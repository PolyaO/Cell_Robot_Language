#pragma once
#include <backend/exprs.hpp>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

#include "interpreter/ast.hpp"
#include "interpreter/defs.hpp"
#include "parser.hpp"

#define YY_DECL yy::parser::symbol_type yylex(ast::AstMaker &ast)

namespace ast {
class AstMaker {
   public:
    using var_dict_t = std::vector<std::tuple<std::string, unsigned, unsigned>>;
    Ast &&get_ast();
    yy::location loc;

    std::variant<unsigned, std::vector<unsigned>> make_arg_list();
    void add_to_arg_list(std::variant<unsigned, std::vector<unsigned>> &list,
                         std::string_view arg);

    std::vector<unsigned> make_dim_list();
    void add_to_dim_list(std::vector<unsigned> &list, unsigned dim);

    std::vector<unsigned> make_stmts();
    void add_to_stmts(std::vector<unsigned> &stmts, unsigned expr_);

    void set_politely_asked(unsigned expr_);

    unsigned make_task(
        std::string_view task_name,
        const std::variant<unsigned, std::vector<unsigned>> &arg_list,
        std::vector<unsigned> &&exprs, std::string_view res);
    unsigned make_findexit(std::vector<unsigned> &&exprs);

    template <class T>
        requires std::same_as<std::decay_t<T>, int> ||
                 std::same_as<std::decay_t<T>, bool>
    unsigned make_var(std::string_view var_name,
                      const std::vector<unsigned> &dim_list, T val) {
        check_for_varname_redeclaration(var_name);
        unsigned idx1 = _ast.make_rval<Var>(val, dim_list);
        unsigned idx2 = _ast.make_rval<Var>(val, dim_list);
        unsigned assign = _ast.make_expr<Assign>(idx1, idx2, loc.begin.line);
        _variables_avaliable.emplace_back(var_name, idx1, assign);
        return assign;
    }

    template <class T>
        requires std::same_as<std::decay_t<T>, Logitize> ||
                 std::same_as<std::decay_t<T>, Digitize>
    unsigned make_transform(std::string_view var_name) {
        auto it = check_for_var_unknown(var_name);
        return _ast.make_expr<T>(std::get<1>(*it));
    }

    unsigned make_assignement(std::string_view var_name,
                              std::vector<unsigned> &&dim_list,
                              unsigned rval_idx);
    template <class T>
        requires std::same_as<std::decay_t<T>, Move> ||
                 std::same_as<std::decay_t<T>, RotateL> ||
                 std::same_as<std::decay_t<T>, RotateR>
    unsigned make_robot_expr() {
        return _ast.make_expr<T>(loc.begin.line);
    }

    unsigned make_do(std::string_view task_name,
                     std::variant<unsigned, std::vector<unsigned>> &&arg_list);

    unsigned make_for(std::string_view counter, std::string_view boundary,
                      std::string_view step, unsigned stmt);
    unsigned make_switch(unsigned rval_idx, bool condition1, unsigned stmt1,
                         bool condition2, unsigned stmt2);
    unsigned make_scope(std::vector<unsigned> &&exprs);

    unsigned make_and(unsigned rval1_idx, unsigned rval2_idx);
    unsigned make_or(unsigned rval1_idx, unsigned rval2_idx);
    unsigned make_sum(unsigned rval1_idx, unsigned rval2_idx);
    unsigned make_sub(unsigned rval1_idx, unsigned rval2_idx);
    unsigned make_div(unsigned rval1_idx, unsigned rval2_idx);
    unsigned make_mul(unsigned rval1_idx, unsigned rval2_idx);

    unsigned make_not(unsigned rval_idx);
    unsigned make_mxtrue(unsigned rval_idx);
    unsigned make_mxfalse(unsigned rval_idx);
    unsigned make_mxeq(unsigned rval_idx);
    unsigned make_mxlt(unsigned rval_idx);
    unsigned make_mxgt(unsigned rval_idx);
    unsigned make_mxlte(unsigned rval_idx);
    unsigned make_mxgte(unsigned rval_idx);
    unsigned make_eleq(unsigned rval_idx);
    unsigned make_ellt(unsigned rval_idx);
    unsigned make_elgt(unsigned rval_idx);
    unsigned make_ellte(unsigned rval_idx);
    unsigned make_elgte(unsigned rval_idx);
    unsigned make_size(unsigned rval_idx);

    unsigned make_reduce(unsigned rval_idx, unsigned dim);
    unsigned make_extend(unsigned rval_idx, unsigned dim);
    unsigned make_ref(std::string_view var_name);
    unsigned make_res(std::string_view task_name);
    unsigned make_env();
    unsigned make_idx(unsigned rval_idx, std::vector<unsigned> &&dim_list);

   private:
    Ast _ast;
    var_dict_t _variables_avaliable;
    var_dict_t::const_iterator find_var(std::string_view var);
    var_dict_t::const_iterator find_var(unsigned assign_idx);
    var_dict_t::const_iterator check_for_var_unknown(std::string_view var);
    void add_var(std::string_view str, unsigned var_idx, unsigned assign_idx);
    void check_for_varname_redeclaration(std::string_view var_name);
    void check_for_taskname_redeclaration(std::string_view task_name);
    void remove_scope_variables(const std::vector<unsigned> &vec);
    variables_t make_task_variables(var_dict_t &&dict);
};
}  // namespace ast
YY_DECL;
