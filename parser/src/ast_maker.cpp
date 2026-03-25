#include <iostream>
#include <string_view>

#include "ast_maker.hpp"

void AstMaker::set_successfully_parsed(bool success) {
    successfully_parsed = success;
}
bool AstMaker::get_successfully_parsed() { return successfully_parsed; }

int AstMaker::new_arg_list(std::string_view arg) {
    std::cout << loc << " New arg list created with arg: " << arg << std::endl;
    return 0;
}

int AstMaker::new_arg_list() {
    std::cout << loc << " New arg list created without args " << std::endl;
    return 0;
}

int AstMaker::new_dim_list(int dim) {
    std::cout << loc << " New dim list created with dim: " << dim << std::endl;
    return 0;
}

void AstMaker::add_to_scope(int scope, int expr) {
    std::cout << loc << " Expr: " << expr << " added to scope: " << scope << std::endl;
}
void AstMaker::add_to_arg_list(int list, std::string_view arg) {
    std::cout << loc << " Arg: " << arg << " added to arg_list: " << list << std::endl;
}
void AstMaker::add_to_dim_list(int list, int dim) {
    std::cout << loc << " Dim: " << dim << " added to dim_list: " << list << std::endl;
}

void AstMaker::set_politely_asked(int expr) {
    std::cout << loc << " Set politely asked" << expr << std::endl;
}

int AstMaker::make_task(std::string_view task_name, int arg_list, int exprs,
                        std::string_view res) {
    std::cout << loc << " Task " << task_name << "made. store res to " << res
              << "; arglist: " << arg_list << "; exprs: " << exprs << std::endl;
    return 1;
}
int AstMaker::make_int_var(std::string_view var_name, int val) {
    std::cout << loc << " IntVar " << var_name << "= " << val << std::endl;
    return 2;
}
int AstMaker::make_bool_var(std::string_view var_name, bool val) {
    std::cout << loc << " BoolVar " << var_name << "= " << val << std::endl;
    return 3;
}
int AstMaker::make_int_var(std::string_view var_name, int dim_list, int val) {
    std::cout << loc << " IntVar" << var_name << "=" << val << "; dim_list: " << dim_list
              << std::endl;
    return 2;
}
int AstMaker::make_bool_var(std::string_view var_name, int dim_list, bool val) {
    std::cout << loc << " BoolVar" << var_name << "=" << val << "; dim_list: " << dim_list
              << std::endl;
    return 3;
}

int AstMaker::make_logitize(std::string_view var_name) {
    std::cout << loc << " logitize " << var_name << std::endl;
    return 4;
}

int AstMaker::make_digitize(std::string_view var_name) {
    std::cout << loc << " digitize " << var_name << std::endl;
    return 5;
}

int AstMaker::make_assignement(std::string_view var_name, int rval) {
    std::cout << loc << " assignement " << var_name << "=" << rval << std::endl;
    return 6;
}

int AstMaker::make_assignement(std::string_view var_name, int dim_list, int rval) {
    std::cout << loc << " assignement " << var_name  << "[" << dim_list << "] =" << rval << std::endl;
    return 6;
}
int AstMaker::make_move() {
    std::cout << loc << " move" << std::endl;
    return 7;
}
int AstMaker::make_rotate_left() {
    std::cout << loc << " rotate left" << std::endl;
    return 8;
}
int AstMaker::make_rotate_right() {
    std::cout << loc << " rotate right" << std::endl;
    return 9;
}
int AstMaker::make_task_call(std::string_view task_name, int arg_list) {
    std::cout << loc << " Task call " << task_name << "; arglist: " << arg_list
              << std::endl;
    return 10;
}

int AstMaker::make_for(std::string_view counter, std::string_view boundary,
                       std::string_view step, int exprs) {
    std::cout << loc << " Make for " << counter << " boundary: " << boundary
              << " step: " << step << " exprs: " << exprs << std::endl;
    return 11;
}
int AstMaker::make_switch(int rval, bool condition, int exprs) {
    std::cout << loc << " Make switch " << rval << "," << condition << ","
              << exprs << std::endl;
    return 12;
}

int AstMaker::make_switch(int rval, bool condition1, int exprs1,
                          bool condition2, int exprs2) {
    std::cout << loc << "Make switch " << rval << "," << condition1 << ","
              << exprs1 << ", " << condition2 << ", " << exprs2 << std::endl;
    return 12;
}

int AstMaker::make_scope() {
    std::cout << loc << " Make scope " << std::endl;
    return 0;
}

int AstMaker::make_and(int rval1, int rval2) {
    std::cout << loc << rval1 << " and " << rval2 << std::endl;
    return 13;
}

int AstMaker::make_or(int rval1, int rval2) {
    std::cout << loc << rval1 << " or " << rval2 << std::endl;
    return 13;
}

int AstMaker::make_sum(int rval1, int rval2) {
    std::cout << loc << rval1 << " + " << rval2 << std::endl;
    return 14;
}
int AstMaker::make_sub(int rval1, int rval2) {
    std::cout << loc << rval1 << " - " << rval2 << std::endl;
    return 15;
}
int AstMaker::make_div(int rval1, int rval2) {
    std::cout << loc << rval1 << " / " << rval2 << std::endl;
    return 16;
}

int AstMaker::make_mul(int rval1, int rval2) {
    std::cout << loc << rval1 << " * " << rval2 << std::endl;
    return 17;
}

int AstMaker::make_not(int rval) {
    std::cout << loc << "not " << rval;
    return 18;
}

int AstMaker::make_mxtrue(int rval) {
    std::cout << loc << "mxtrue " << rval;
    return 18;
}

int AstMaker::make_mxfalse(int rval) {
    std::cout << loc << "mxfalse " << rval;
    return 18;
}
int AstMaker::make_mxeq(int rval) {
    std::cout << loc << "mxeq " << rval;
    return 18;
}
int AstMaker::make_mxlt(int rval) {
    std::cout << loc << "mxlt " << rval;
    return 18;
}
int AstMaker::make_mxgt(int rval) {
    std::cout << loc << "mxgt " << rval;
    return 18;
}
int AstMaker::make_mxlte(int rval) {
    std::cout << loc << "mxlte " << rval;
    return 18;
}
int AstMaker::make_mxgte(int rval) {
    std::cout << loc << "mxgte " << rval;
    return 18;
}
int AstMaker::make_eleq(int rval) {
    std::cout << loc << "eleq " << rval;
    return 18;
}
int AstMaker::make_ellt(int rval) {
    std::cout << loc << "ellt " << rval;
    return 18;
}
int AstMaker::make_elgt(int rval) {
    std::cout << loc << "elgt " << rval;
    return 18;
}
int AstMaker::make_ellte(int rval) {
    std::cout << loc << "ellte " << rval;
    return 18;
}
int AstMaker::make_elgte(int rval) {
    std::cout << loc << "elgte " << rval;
    return 18;
}
int AstMaker::make_size(int rval) {
    std::cout << loc << " size " << rval;
    return 18;
}

int AstMaker::make_reduce(int unary_op, int dim) {
    std::cout << loc << " reduce " << unary_op << ", " << dim << std::endl;
    return 20;
}
int AstMaker::make_extend(int unary_op, int dim) {
    std::cout << loc << " extend " << unary_op << ", " << dim << std::endl;
    return 20;
}
int AstMaker::make_mx_rval(std::string_view id) {
    std::cout << loc << " id rval " << id << std::endl;
    return 20;
}
int AstMaker::make_get_task_res(std::string_view task_name) {
    std::cout << loc << " get task res " << task_name << std::endl;
    return 20;
}
int AstMaker::make_get_env() {
    std::cout << loc << " get env " << std::endl;
    return 20;
}
int AstMaker::make_idx(int rval, int dim_list) {
    std::cout << loc << " idx " << rval << ", " << dim_list << std::endl;
    return 20;
}
