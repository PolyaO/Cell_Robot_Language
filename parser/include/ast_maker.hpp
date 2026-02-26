#pragma once
#include <string_view>
#include "parser.hpp"

#define YY_DECL yy::parser::symbol_type yylex(AstMaker &ast)

YY_DECL;

class AstMaker {
   public:
    yy::location loc;

    void set_successfully_parsed(bool success);
    bool get_successfully_parsed();

    int new_arg_list(std::string_view arg);
    int new_arg_list();
    int new_dim_list(int dim);
    void add_to_scope(int scope, int expr);
    void add_to_arg_list(int list, std::string_view arg);
    void add_to_dim_list(int list, int dim);

    void set_politely_asked(int expr);

    int make_task(std::string_view task_name, int arg_list, int exprs,
                  std::string_view res);
    int make_int_var(std::string_view var_name, int val);
    int make_bool_var(std::string_view var_name, bool val);
    int make_int_var(std::string_view var_name, int dim_list, int val);
    int make_bool_var(std::string_view var_name, int dim_list, bool val);

    int make_logitize(std::string_view var_name);
    int make_digitize(std::string_view var_name);

    int make_assignement(std::string_view var_name, int rval);
    int make_move();
    int make_rotate_left();
    int make_rotate_right();
    int make_task_call(std::string_view task_name, int arg_list);

    int make_for(std::string_view counter, std::string_view boundary,
                 std::string_view step, int exprs);
    int make_switch(int rval, bool condition, int exprs);
    int make_switch(int rval, bool condition1, int exprs1, bool condition2,
                    int exprs2);
    int make_scope();

    int make_and(int rval1, int rval2);
    int make_or(int rval1, int rval2);
    int make_sum(int rval1, int rval2);
    int make_sub(int rval1, int rval2);
    int make_div(int rval1, int rval2);
    int make_mul(int rval1, int rval2);

    int make_not(int rval);
    int make_mxtrue(int rval);
    int make_mxfalse(int rval);
    int make_mxeq(int rval);
    int make_mxlt(int rval);
    int make_mxgt(int rval);
    int make_mxlte(int rval);
    int make_mxgte(int rval);
    int make_eleq(int rval);
    int make_ellt(int rval);
    int make_elgt(int rval);
    int make_ellte(int rval);
    int make_elgte(int rval);
    int make_size(int rval);

    int make_reduce(int unary_op, int dim);
    int make_extend(int unary_op, int dim);
    int make_mx_rval(std::string_view id);
    int make_get_task_res(std::string_view task_name);
    int make_get_env();
    int make_idx(int rval, int dim_list);

   private:
    bool successfully_parsed = false;
};
