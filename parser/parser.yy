%skeleton "lalr1.cc"
%require "3.8.1"
%language "c++"
%header
%define api.value.type variant
%define api.token.constructor
%define parse.assert

%code requires {
    #include <string>
    #include <variant>
    class AstMaker;
}

%locations
%define parse.trace
%define parse.error detailed
%define parse.lac full
%param { AstMaker& ast }

%code {
    #include "ast_maker.hpp"
}
%define api.token.prefix {TOK_}
%token <std::string> IDENTIFIER "identifier"
%token <int> INTEGER "integer"
%token <bool> BOOLEAN "boolean"
%token TASK RESULT DO GET FOR BOUNDARY STEP SWITCH
%token VAR SIZE LOGITIZE DIGITIZE REDUCE EXTEND
%token MOVE ROTATE_LEFT ROTATE_RIGHT GET_ENVIRONMENT
%token PLEASE THANKS
%left AND
%left '+' '-'
%left '*' '/'
%right NOT MXTRUE MXFALSE MXEQ MXLT MXGT MXLTE MXGTE ELEQ ELLT ELGT ELLTE ELGTE GET SIZE REDUCE EXTEND
%nonassoc '=' '[' ']' '(' ')'
%type <int> expr rval scope stmts stmt complex_expr_stmt complex_expr_stmts dim_list arg_list unary primary task
%nterm programm tasks

%%
programm
    : %empty                        {ast.set_successfully_parsed(true);}
    | tasks                         {ast.set_successfully_parsed(true);}
    ;
tasks
    : task                             {}
    | tasks task                       {}
    ;
task
    : TASK IDENTIFIER arg_list '(' '\n' stmts  RESULT IDENTIFIER '\n' ')' '\n' {$$ = ast.make_task($2, $3, $6, $8);}
    ;
scope
    : '(' '\n' stmts ')' '\n'           {$$ = $3;}
    ;
stmts
    : stmt                             {$$ = ast.new_body(); ast.add_to_body($$, $1);}
    | stmts stmt                       {$$ = $1; ast.add_to_body($$, $2);}
    | complex_expr_stmts               {$$ = ast.new_body(); ast.add_to_body($$, $1);}
    | stmts complex_expr_stmts         {$$ = $1; ast.add_to_body($$, $2);}
    ;
stmt
    : PLEASE expr THANKS '\n'           {ast.set_politely_asked($2); $$ = $2;}
    | expr '\n'                         {$$ = $1;}
    | complex_expr_stmt                 {$$ = $1;}
    ;
expr
    : VAR IDENTIFIER '=' INTEGER                     {$$ = ast.make_int_var($2, $4);}
    | VAR IDENTIFIER '=' BOOLEAN                     {$$ = ast.make_bool_var($2, $4);}
    | VAR IDENTIFIER '[' dim_list ']' '=' INTEGER    {$$ = ast.make_int_var($2, $4, $7);}
    | VAR IDENTIFIER '[' dim_list ']' '=' BOOLEAN    {$$ = ast.make_bool_var($2, $4, $7);}
    | LOGITIZE IDENTIFIER   {$$ = ast.make_logitize($2);}
    | DIGITIZE IDENTIFIER   {$$ = ast.make_digitize($2);}
    | IDENTIFIER '=' rval   {$$ = ast.make_assignement($1, $3);}
    | MOVE                  {$$ = ast.make_move();}
    | ROTATE_LEFT           {$$ = ast.make_rotate_left();}
    | ROTATE_RIGHT          {$$ = ast.make_rotate_right();}
    | DO IDENTIFIER arg_list {$$ = ast.make_task_call($2, $3);}
    ;
complex_expr_stmt
    : FOR IDENTIFIER BOUNDARY IDENTIFIER STEP IDENTIFIER stmt {$$ = ast.make_for($2, $4, $6, $7);}
    | SWITCH rval BOOLEAN stmt                                {$$ = ast.make_switch($2, $3, $4);}
    ;
complex_expr_stmts
    : FOR IDENTIFIER BOUNDARY IDENTIFIER STEP IDENTIFIER scope {$$ = ast.make_for($2, $4, $6, $7);}
    | SWITCH rval BOOLEAN scope                                {$$ = ast.make_switch($2, $3, $4);}
    | SWITCH rval BOOLEAN scope BOOLEAN scope                  {$$ = ast.make_switch($2, $3, $4, $5, $6);}
    ;
dim_list
    : INTEGER              {$$ = ast.new_dim_list($1);}
    | dim_list ',' INTEGER {$$ = $1; ast.add_to_dim_list($1, $3);}
    ;
arg_list: %empty              {$$ = ast.new_arg_list();}
    | IDENTIFIER              {$$ = ast.new_arg_list($1);}
    | arg_list ',' IDENTIFIER {$$ = $1; ast.add_to_arg_list($1, $3);}
    ;
rval
    : rval AND rval   {$$ = ast.make_and($1, $3);}
    | rval '+' rval   {$$ = ast.make_sum($1, $3);}
    | rval '-' rval   {$$ = ast.make_sub($1, $3);}
    | rval '*' rval   {$$ = ast.make_mul($1, $3);}
    | rval '/' rval   {$$ = ast.make_div($1, $3);}
    | unary           {$$ = $1;}
    ;
unary
    : primary         %prec AND {$$ = $1;}
    | NOT unary       {$$ = ast.make_not($2);}
    | MXTRUE unary    {$$ = ast.make_mxtrue($2);}
    | MXFALSE unary   {$$ = ast.make_mxfalse($2);}
    | MXEQ unary      {$$ = ast.make_mxeq($2);}
    | MXLT unary      {$$ = ast.make_mxlt($2);}
    | MXGT unary      {$$ = ast.make_mxgt($2);}
    | MXLTE unary     {$$ = ast.make_mxlte($2);}
    | MXGTE unary     {$$ = ast.make_mxgte($2);}
    | ELEQ unary      {$$ = ast.make_eleq($2);}
    | ELLT unary      {$$ = ast.make_ellt($2);}
    | ELGT unary      {$$ = ast.make_elgt($2);}
    | ELLTE unary     {$$ = ast.make_ellte($2);}
    | ELGTE unary     {$$ = ast.make_elgte($2);}
    | SIZE unary      {$$ = ast.make_size($2);}
    | REDUCE unary '[' INTEGER ']' {$$ = ast.make_reduce($2, $4);}
    | EXTEND unary '[' INTEGER ']' {$$ = ast.make_extend($2, $4);}
    ;
primary
    : IDENTIFIER                {$$ = ast.make_mx_rval($1);}
    | GET IDENTIFIER            {$$ = ast.make_get_task_res($2);}
    | GET_ENVIRONMENT           {$$ = ast.make_get_env();}
    | '(' rval ')'              {$$ = $2;}
    | primary '[' dim_list ']'  {$$ = ast.make_idx($1, $3);}
    ;
%%
