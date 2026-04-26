#pragma once
#include <string_view>
#include <vector>

#include "gen/location.hh"
#include "gen/parser.hpp"
#include "interpreter/ast.hpp"
#include "interpreter/exceptions/build_exceptions.hpp"
#include "interpreter/exprs/declare.hpp"

#define YY_DECL yy::parser::symbol_type yylex(ast::AstMaker &ast)

namespace ast {
class AstMaker {
   public:
    yy::location loc;

    Ast get_ast();
    std::variant<unsigned, std::vector<unsigned>> make_arg_list() const;
    void add_to_arg_list(std::variant<unsigned, std::vector<unsigned>> &list,
                         std::string_view arg, unsigned line);

    std::vector<unsigned> make_dim_list() const;
    void add_to_dim_list(std::vector<unsigned> &list, int dim,
                         unsigned line) const;

    std::vector<unsigned> make_stmts() const;
    void add_to_stmts(std::vector<unsigned> &stmts, unsigned expr_) const;

    unsigned make_task(
        std::string_view task_name,
        const std::variant<unsigned, std::vector<unsigned>> &arg_list,
        std::vector<unsigned> &&exprs, std::string_view res, unsigned line);

    unsigned make_findexit(std::vector<unsigned> &&exprs, unsigned line);

    unsigned make_assignement(std::string_view var_name,
                              std::vector<unsigned> &&dim_list,
                              unsigned rval_idx, unsigned line);

    template <class T>
        requires std::same_as<std::decay_t<T>, int> ||
                 std::same_as<std::decay_t<T>, bool_t>
    unsigned make_val_assignement(std::string_view var_name,
                                  std::vector<unsigned> &&dim_list, T val,
                                  unsigned line);

    template <class T>
        requires std::same_as<std::decay_t<T>, int> ||
                 std::same_as<std::decay_t<T>, bool_t>
    unsigned make_var_declaration(std::string_view var_name,
                                  const std::vector<unsigned> &dim_list, T val,
                                  unsigned line);

    template <class T>
        requires std::same_as<std::decay_t<T>, Logitize> ||
                 std::same_as<std::decay_t<T>, Digitize>
    unsigned make_transform(std::string_view var_name, unsigned line);

    template <class T>
        requires std::same_as<std::decay_t<T>, Reduce> ||
                 std::same_as<std::decay_t<T>, Extend>
    unsigned make_change(unsigned rval_idx, int dim, int change, unsigned line);

    template <class T>
        requires std::same_as<std::decay_t<T>, Move> ||
                 std::same_as<std::decay_t<T>, RotateL> ||
                 std::same_as<std::decay_t<T>, RotateR>
    unsigned make_robot_expr(unsigned line);

    unsigned make_do(std::string_view task_name,
                     std::variant<unsigned, std::vector<unsigned>> &&arg_list,
                     unsigned line);

    unsigned make_for(std::string_view counter, std::string_view boundary,
                      std::string_view step, unsigned stmt, unsigned line);
    unsigned make_switch(unsigned rval_idx, bool condition1, unsigned stmt1,
                         bool condition2, unsigned stmt2, unsigned line);
    unsigned make_scope(std::vector<unsigned> &&exprs, unsigned line);

    unsigned make_and(unsigned rval1_idx, unsigned rval2_idx, unsigned line);
    unsigned make_or(unsigned rval1_idx, unsigned rval2_idx, unsigned line);
    unsigned make_sum(unsigned rval1_idx, unsigned rval2_idx, unsigned line);
    unsigned make_sub(unsigned rval1_idx, unsigned rval2_idx, unsigned line);
    unsigned make_div(unsigned rval1_idx, unsigned rval2_idx, unsigned line);
    unsigned make_mul(unsigned rval1_idx, unsigned rval2_idx, unsigned line);

    unsigned make_not(unsigned rval_idx, unsigned line);
    unsigned make_mxtrue(unsigned rval_idx, unsigned line);
    unsigned make_mxfalse(unsigned rval_idx, unsigned line);
    unsigned make_mxeq(unsigned rval_idx, unsigned line);
    unsigned make_mxlt(unsigned rval_idx, unsigned line);
    unsigned make_mxgt(unsigned rval_idx, unsigned line);
    unsigned make_mxlte(unsigned rval_idx, unsigned line);
    unsigned make_mxgte(unsigned rval_idx, unsigned line);
    unsigned make_eleq(unsigned rval_idx, unsigned line);
    unsigned make_ellt(unsigned rval_idx, unsigned line);
    unsigned make_elgt(unsigned rval_idx, unsigned line);
    unsigned make_ellte(unsigned rval_idx, unsigned line);
    unsigned make_elgte(unsigned rval_idx, unsigned line);
    unsigned make_size(unsigned rval_idx, unsigned line);

    unsigned make_ref(std::string_view var_name, unsigned line);
    unsigned make_res(std::string_view task_name, unsigned line);
    unsigned make_env(unsigned line);
    unsigned make_idx(unsigned rval_idx, std::vector<unsigned> &&dim_list,
                      unsigned line);
    void set_politely_asked(unsigned expr_);

   private:
    void process_task_calls(std::string_view task_name);
    unsigned make_task(std::string_view task_name,
                       std::vector<unsigned> &&exprs, unsigned args_num,
                       unsigned res_ref_idx, unsigned line);


    struct VarInfo {
        Ast::VarMetainf metainf;
        unsigned declare_idx;
        unsigned declare_line;
    };

    struct TaskCallInfo {
        unsigned do_idx;
        unsigned line;
        std::string task_name;
    };

    class VariablesDict {
       public:
        const VarInfo *get_var_info(std::string_view var_name) const;
        void add_var_info(VarInfo &&info);
        void remove_declared(const std::vector<unsigned> &exprs, Ast::vars_metainf_t &dst);
        bool empty() const;
        Ast::vars_metainf_t clear();

       private:
        using var_dict_t = std::vector<VarInfo>;
        var_dict_t _variables_avaliable;
    };

    using task_calls_t = std::vector<TaskCallInfo>;
    Ast _ast;
    Ast::vars_metainf_t _task_vars;
    task_calls_t _unprocessed_task_calls;
    VariablesDict _variables_avaliable;
    unsigned _vars_number = 0;
    unsigned _counters_number = 0;
};

template <class T>
    requires std::same_as<std::decay_t<T>, int> ||
             std::same_as<std::decay_t<T>, bool_t>
unsigned AstMaker::make_val_assignement(std::string_view var_name,
                                        std::vector<unsigned> &&dim_list, T val,
                                        unsigned line) {
    auto var_info_ptr = _variables_avaliable.get_var_info(var_name);
    if (!var_info_ptr) throw VarUnknown(var_name, line);
    unsigned idx = var_info_ptr->metainf.ref_idx;
    if (!dim_list.empty())
        idx = _ast.make_rval<Idx>(idx, std::move(dim_list), line);

    return _ast.make_expr<AssignVal>(idx, val, line);
}

template <class T>
    requires std::same_as<std::decay_t<T>, int> ||
             std::same_as<std::decay_t<T>, bool_t>
unsigned AstMaker::make_var_declaration(std::string_view var_name,
                                        const std::vector<unsigned> &dim_list,
                                        T val, unsigned line) {
    auto var_info_ptr = _variables_avaliable.get_var_info(var_name);
    if (var_info_ptr)
        throw VarRedeclare(var_name, var_info_ptr->declare_line, line);
    unsigned ref_idx = _ast.make_rval<Ref>(_vars_number, line);
    unsigned decl_idx =
        _ast.make_expr<Declare>(_vars_number, dim_list, val, line);
    Ast::VarMetainf m(std::string(var_name), ref_idx, _vars_number);
    VarInfo info(std::move(m), decl_idx, line);
    _variables_avaliable.add_var_info(std::move(info));
    _vars_number++;
    return decl_idx;
}

template <class T>
    requires std::same_as<std::decay_t<T>, Logitize> ||
             std::same_as<std::decay_t<T>, Digitize>
unsigned AstMaker::make_transform(std::string_view var_name, unsigned line) {
    auto var_info_ptr = _variables_avaliable.get_var_info(var_name);
    if (!var_info_ptr) throw VarUnknown(var_name, line);
    return _ast.make_expr<T>(var_info_ptr->metainf.ref_idx, line);
}

template <class T>
    requires std::same_as<std::decay_t<T>, Reduce> ||
             std::same_as<std::decay_t<T>, Extend>
unsigned AstMaker::make_change(unsigned rval_idx, int dim, int change,
                               unsigned line) {
    if (dim <= 0) throw WrongDimIdx(dim, line);
    if (change < 0) throw WrongChange(change, line);
    return _ast.make_rval<T>(rval_idx, dim, change, line);
}

template <class T>
    requires std::same_as<std::decay_t<T>, Move> ||
             std::same_as<std::decay_t<T>, RotateL> ||
             std::same_as<std::decay_t<T>, RotateR>
unsigned AstMaker::make_robot_expr(unsigned line) {
    return _ast.make_expr<T>(line);
}
}  // namespace ast

YY_DECL;
