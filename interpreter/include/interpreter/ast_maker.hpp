#pragma once
#include <string_view>
#include <vector>

#include "gen/location.hh"
#include "gen/parser.hpp"
#include "interpreter/ast.hpp"
#include "interpreter/exceptions/build_exceptions.hpp"

#define YY_DECL yy::parser::symbol_type yylex(ast::AstMaker &ast)

namespace ast {
class AstMaker {
   public:
    Ast get_ast();
    std::variant<unsigned, std::vector<unsigned>> make_arg_list() const;
    void add_to_arg_list(std::variant<unsigned, std::vector<unsigned>> &list,
                         std::string_view arg, unsigned lineno);

    std::vector<unsigned> make_dim_list() const;
    void add_to_dim_list(std::vector<unsigned> &list, int dim,
                         unsigned lineno) const;

    std::vector<unsigned> make_stmts() const;
    void add_to_stmts(std::vector<unsigned> &stmts, unsigned expr_) const;

    unsigned make_task(
        std::string_view task_name,
        const std::variant<unsigned, std::vector<unsigned>> &arg_list,
        std::vector<unsigned> &&exprs, unsigned lineno);

    unsigned make_findexit(std::vector<unsigned> &&exprs, unsigned lineno);

    unsigned make_assignement(std::string_view var_name,
                              std::vector<unsigned> &&dim_list,
                              unsigned rval_idx, unsigned lineno);

    template <class T>
        requires std::same_as<std::decay_t<T>, int> ||
                 std::same_as<std::decay_t<T>, bool_t>
    unsigned make_val_assignement(std::string_view var_name,
                                  std::vector<unsigned> &&dim_list, T val,
                                  unsigned lineno);

    template <class T>
        requires std::same_as<std::decay_t<T>, int> ||
                 std::same_as<std::decay_t<T>, bool_t>
    unsigned make_var_declaration(std::string_view var_name,
                                  const std::vector<unsigned> &dim_list, T val,
                                  unsigned lineno);

    template <class T>
        requires std::same_as<std::decay_t<T>, exprs::Logitize> ||
                 std::same_as<std::decay_t<T>, exprs::Digitize>
    unsigned make_transform(std::string_view var_name, unsigned lineno);

    template <class T>
        requires std::same_as<std::decay_t<T>, rvals::Reduce> ||
                 std::same_as<std::decay_t<T>, rvals::Extend>
    unsigned make_change(unsigned rval_idx, int dim, int change,
                         unsigned lineno);

    template <class T>
        requires std::same_as<std::decay_t<T>, exprs::Move> ||
                 std::same_as<std::decay_t<T>, exprs::RotateL> ||
                 std::same_as<std::decay_t<T>, exprs::RotateR>
    unsigned make_robot_expr(unsigned lineno);

    unsigned make_do(std::string_view task_name,
                     std::variant<unsigned, std::vector<unsigned>> &&arg_list,
                     unsigned lineno);
    unsigned make_ret_res(std::string_view res_name, unsigned lineno);

    unsigned make_for(std::string_view counter, std::string_view boundary,
                      std::string_view step, unsigned stmt, unsigned lineno);
    unsigned make_switch(unsigned rval_idx, bool condition1, unsigned stmt1,
                         bool condition2, unsigned stmt2, unsigned lineno);
    unsigned make_switch_no_tail(unsigned rval_idx, bool condition,
                                 unsigned stmt, unsigned lineno);
    unsigned make_scope(std::vector<unsigned> &&exprs, unsigned lineno);

    unsigned make_and(unsigned rval1_idx, unsigned rval2_idx, unsigned lineno);
    unsigned make_or(unsigned rval1_idx, unsigned rval2_idx, unsigned lineno);
    unsigned make_sum(unsigned rval1_idx, unsigned rval2_idx, unsigned lineno);
    unsigned make_sub(unsigned rval1_idx, unsigned rval2_idx, unsigned lineno);
    unsigned make_div(unsigned rval1_idx, unsigned rval2_idx, unsigned lineno);
    unsigned make_mul(unsigned rval1_idx, unsigned rval2_idx, unsigned lineno);

    unsigned make_not(unsigned rval_idx, unsigned lineno);
    unsigned make_mxtrue(unsigned rval_idx, unsigned lineno);
    unsigned make_mxfalse(unsigned rval_idx, unsigned lineno);
    unsigned make_mxeq(unsigned rval_idx, unsigned lineno);
    unsigned make_mxlt(unsigned rval_idx, unsigned lineno);
    unsigned make_mxgt(unsigned rval_idx, unsigned lineno);
    unsigned make_mxlte(unsigned rval_idx, unsigned lineno);
    unsigned make_mxgte(unsigned rval_idx, unsigned lineno);
    unsigned make_eleq(unsigned rval_idx, unsigned lineno);
    unsigned make_ellt(unsigned rval_idx, unsigned lineno);
    unsigned make_elgt(unsigned rval_idx, unsigned lineno);
    unsigned make_ellte(unsigned rval_idx, unsigned lineno);
    unsigned make_elgte(unsigned rval_idx, unsigned lineno);
    unsigned make_size(unsigned rval_idx, unsigned lineno);

    unsigned make_ref(std::string_view var_name, unsigned lineno);
    unsigned make_res(std::string_view task_name, unsigned lineno);
    unsigned make_env(unsigned lineno);
    unsigned make_idx(unsigned rval_idx, std::vector<unsigned> &&dim_list,
                      unsigned lineno);
    void set_politely_asked(unsigned expr_);

    yy::location loc;

   private:
    struct VarInfo {
        Ast::VarMetainf metainf;
        unsigned declare_idx;
        unsigned declare_lineno;
    };

    struct TaskCallInfo {
        unsigned do_idx;
        unsigned lineno;
        std::string task_name;
    };

    using task_calls_t = std::vector<TaskCallInfo>;

    class VariablesDict {
       public:
        const VarInfo *get_var_info(std::string_view var_name) const;
        void add_var_info(VarInfo &&info);
        void remove_declared(const std::vector<unsigned> &exprs,
                             Ast::vars_metainf_t &dst);
        bool empty() const;
        Ast::vars_metainf_t clear();

       private:
        using var_dict_t = std::vector<VarInfo>;
        var_dict_t _variables_avaliable;
    };

    void process_task_calls(std::string_view task_name);
    void make_task_metainf(std::string_view task_name, unsigned task_idx,
                           unsigned args_num, unsigned lineno);

    Ast _ast;
    Ast::vars_metainf_t _task_vars;
    task_calls_t _unprocessed_task_calls;
    VariablesDict _variables_avaliable;
    unsigned _vars_number = 0;
    unsigned _counters_number = 0;
    bool _met_res = false;
};

template <class T>
    requires std::same_as<std::decay_t<T>, int> ||
             std::same_as<std::decay_t<T>, bool_t>
unsigned AstMaker::make_val_assignement(std::string_view var_name,
                                        std::vector<unsigned> &&dim_list, T val,
                                        unsigned lineno) {
    auto var_info_ptr = _variables_avaliable.get_var_info(var_name);
    if (!var_info_ptr) throw VarUnknown(var_name, lineno);
    unsigned idx = var_info_ptr->metainf.ref_idx;
    if (!dim_list.empty())
        idx = _ast.make_rval<rvals::Idx>(lineno, idx, std::move(dim_list));

    return _ast.make_expr<exprs::AssignVal>(lineno, false, idx, val);
}

template <class T>
    requires std::same_as<std::decay_t<T>, int> ||
             std::same_as<std::decay_t<T>, bool_t>
unsigned AstMaker::make_var_declaration(std::string_view var_name,
                                        const std::vector<unsigned> &dim_list,
                                        T val, unsigned lineno) {
    auto var_info_ptr = _variables_avaliable.get_var_info(var_name);
    if (var_info_ptr)
        throw VarRedeclare(var_name, var_info_ptr->declare_lineno, lineno);
    unsigned ref_idx = _ast.make_rval<rvals::Ref>(lineno, _vars_number);
    unsigned decl_idx = _ast.make_expr<exprs::Declare>(
        lineno, false, _vars_number, dim_list, val);
    Ast::VarMetainf m(std::string(var_name), ref_idx, _vars_number);
    VarInfo info(std::move(m), decl_idx, lineno);
    _variables_avaliable.add_var_info(std::move(info));
    _vars_number++;
    return decl_idx;
}

template <class T>
    requires std::same_as<std::decay_t<T>, exprs::Logitize> ||
             std::same_as<std::decay_t<T>, exprs::Digitize>
unsigned AstMaker::make_transform(std::string_view var_name, unsigned lineno) {
    auto var_info_ptr = _variables_avaliable.get_var_info(var_name);
    if (!var_info_ptr) throw VarUnknown(var_name, lineno);
    return _ast.make_expr<T>(lineno, false, var_info_ptr->metainf.ref_idx);
}

template <class T>
    requires std::same_as<std::decay_t<T>, rvals::Reduce> ||
             std::same_as<std::decay_t<T>, rvals::Extend>
unsigned AstMaker::make_change(unsigned rval_idx, int dim, int change,
                               unsigned lineno) {
    if (dim <= 0) throw WrongDimIdx(dim, lineno);
    if (change < 0) throw WrongChange(change, lineno);
    return _ast.make_rval<T>(lineno, rval_idx, dim, change);
}

template <class T>
    requires std::same_as<std::decay_t<T>, exprs::Move> ||
             std::same_as<std::decay_t<T>, exprs::RotateL> ||
             std::same_as<std::decay_t<T>, exprs::RotateR>
unsigned AstMaker::make_robot_expr(unsigned lineno) {
    return _ast.make_expr<T>(lineno, false);
}
}  // namespace ast

YY_DECL;
