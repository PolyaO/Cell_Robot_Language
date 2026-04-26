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
    namespace ast {
    class AstMaker;
    }
}

%locations
%define parse.trace
%define parse.error detailed
%define parse.lac full
%param { ast::AstMaker& ast }

%code {
    #include "interpreter/ast_maker.hpp"
    #include "var/bool.hpp"
}
%define api.token.prefix {TOK_}
%left LOWEST
%token <std::string> IDENTIFIER "identifier"
%token <int> INTEGER "integer"
%token <bool> BOOLEAN "boolean"
%token TASK FINDEXIT RESULT DO GET FOR BOUNDARY STEP SWITCH
%token VAR SIZE LOGITIZE DIGITIZE REDUCE EXTEND
%token MOVE ROTATE_LEFT ROTATE_RIGHT GET_ENVIRONMENT
%token PLEASE THANKS
%left OR
%left AND
%left '+' '-'
%left '*' '/'
%right NOT MXTRUE MXFALSE MXEQ MXLT MXGT MXLTE MXGTE ELEQ ELLT ELGT ELLTE ELGTE GET SIZE REDUCE EXTEND
%nonassoc '=' '[' ']' '(' ')'
%type <unsigned> expr stmt complex_expr_stmt rval unary primary task any_task find_exit
%type <std::vector<unsigned>> dim_list stmts
%type <std::variant<unsigned, std::vector<unsigned>>> arg_list
%nterm programm tasks newline_opt
%left '\n'
%precedence SWITCH_NO_TAIL
%precedence BOOLEAN

%%
programm
    : %empty                        {}
    | tasks                         {}
    ;
tasks
    : task                             {}
    | tasks task                       {}
    ;
task
    : any_task                         {$$ = $1;}
    | find_exit                        {$$ = $1;}
    ;
any_task
    : TASK IDENTIFIER arg_list '(' newline_opt stmts  RESULT IDENTIFIER '\n' ')' newline_opt {$$ = ast.make_task($2, $3, std::move($6), $8, @1.begin.line);}
    ;
find_exit
    : TASK FINDEXIT '('newline_opt stmts')' newline_opt {$$ = ast.make_findexit(std::move($5), @1.begin.line);}
    ;
expr
    : VAR IDENTIFIER '=' INTEGER                     {$$ = ast.make_var_declaration<int>($2, {}, $4, @1.begin.line);}
    | VAR IDENTIFIER '=' BOOLEAN                     {$$ = ast.make_var_declaration<bool_t>($2, {}, $4, @1.begin.line);}
    | VAR IDENTIFIER '[' dim_list ']' '=' INTEGER    {$$ = ast.make_var_declaration<int>($2, $4, $7, @1.begin.line);}
    | VAR IDENTIFIER '[' dim_list ']' '=' BOOLEAN    {$$ = ast.make_var_declaration<bool_t>($2, $4, $7, @1.begin.line);}
    | LOGITIZE IDENTIFIER   {$$ = ast.make_transform<ast::Logitize>($2, @1.begin.line);}
    | DIGITIZE IDENTIFIER   {$$ = ast.make_transform<ast::Digitize>($2, @1.begin.line);}
    | IDENTIFIER '=' rval   {$$ = ast.make_assignement($1, {}, $3, @1.begin.line);}
    | IDENTIFIER '[' dim_list ']' '=' rval   {$$ = ast.make_assignement($1, std::move($3), $6, @1.begin.line);}
    | IDENTIFIER '=' INTEGER   {$$ = ast.make_val_assignement<int>($1, {}, $3, @1.begin.line);}
    | IDENTIFIER '[' dim_list ']' '=' INTEGER   {$$ = ast.make_val_assignement<int>($1, std::move($3), $6, @1.begin.line);}
    | IDENTIFIER '=' BOOLEAN   {$$ = ast.make_val_assignement<bool_t>($1, {}, $3, @1.begin.line);}
    | IDENTIFIER '[' dim_list ']' '=' BOOLEAN   {$$ = ast.make_val_assignement<bool_t>($1, std::move($3), $6, @1.begin.line);}
    | MOVE                  {$$ = ast.make_robot_expr<ast::Move>( @1.begin.line);}
    | ROTATE_LEFT           {$$ = ast.make_robot_expr<ast::RotateL>( @1.begin.line);}
    | ROTATE_RIGHT          {$$ = ast.make_robot_expr<ast::RotateR>( @1.begin.line);}
    | DO IDENTIFIER arg_list {$$ = ast.make_do($2, std::move($3), @1.begin.line);}
    ;
complex_expr_stmt
    : FOR IDENTIFIER BOUNDARY IDENTIFIER STEP IDENTIFIER newline_opt stmt {$$ = ast.make_for($2, $4, $6, $8, @1.begin.line);}
    | SWITCH rval newline_opt BOOLEAN newline_opt stmt                            %prec SWITCH_NO_TAIL {$$ = ast.make_switch($2, $4, $6, false, 0, @1.begin.line);}
    | SWITCH rval newline_opt BOOLEAN newline_opt stmt BOOLEAN newline_opt stmt   %prec BOOLEAN        {$$ = ast.make_switch($2, $4, $6, $7, $9, @1.begin.line);}
    | '(' newline_opt stmts ')'                               {$$ = ast.make_scope(std::move($3), @1.begin.line);}
    ;
stmt
    : PLEASE expr THANKS '\n' newline_opt    %prec LOWEST {ast.set_politely_asked($2); $$ = $2;}
    | expr '\n' newline_opt                  %prec LOWEST {$$ = $1;}
    | complex_expr_stmt   newline_opt        %prec LOWEST {$$ = $1;}
    ;
stmts
    : stmt                 {$$ = ast.make_stmts(); ast.add_to_stmts($$, $1);}
    | stmts stmt           {$$ = $1; ast.add_to_stmts($$, $2);}
    ;
newline_opt
    : %empty
    | newline_opt '\n'
    ;
dim_list
    : INTEGER              {$$ = ast.make_dim_list(); ast.add_to_dim_list($$, $1, @1.begin.line);}
    | dim_list ',' INTEGER {$$ = $1; ast.add_to_dim_list($$, $3, @1.begin.line);}
    ;
arg_list
    : IDENTIFIER              {$$ = ast.make_arg_list(); ast.add_to_arg_list($$, $1, @1.begin.line);}
    | arg_list ',' IDENTIFIER {$$ = $1; ast.add_to_arg_list($$, $3, @1.begin.line);}
    ;
rval
    : rval AND rval   {$$ = ast.make_and($1, $3, @1.begin.line);}
    | rval OR  rval   {$$ = ast.make_or($1, $3, @1.begin.line);}
    | rval '+' rval   {$$ = ast.make_sum($1, $3, @1.begin.line);}
    | rval '-' rval   {$$ = ast.make_sub($1, $3, @1.begin.line);}
    | rval '*' rval   {$$ = ast.make_mul($1, $3, @1.begin.line);}
    | rval '/' rval   {$$ = ast.make_div($1, $3, @1.begin.line);}
    | rval '[' dim_list ']'  {$$ = ast.make_idx($1, std::move($3), @1.begin.line);}
    | unary           {$$ = $1;}
    ;
unary
    : primary         {$$ = $1;}
    | NOT unary       {$$ = ast.make_not($2, @1.begin.line);}
    | MXTRUE unary    {$$ = ast.make_mxtrue($2, @1.begin.line);}
    | MXFALSE unary   {$$ = ast.make_mxfalse($2, @1.begin.line);}
    | MXEQ unary      {$$ = ast.make_mxeq($2, @1.begin.line);}
    | MXLT unary      {$$ = ast.make_mxlt($2, @1.begin.line);}
    | MXGT unary      {$$ = ast.make_mxgt($2, @1.begin.line);}
    | MXLTE unary     {$$ = ast.make_mxlte($2, @1.begin.line);}
    | MXGTE unary     {$$ = ast.make_mxgte($2, @1.begin.line);}
    | ELEQ unary      {$$ = ast.make_eleq($2, @1.begin.line);}
    | ELLT unary      {$$ = ast.make_ellt($2, @1.begin.line);}
    | ELGT unary      {$$ = ast.make_elgt($2, @1.begin.line);}
    | ELLTE unary     {$$ = ast.make_ellte($2, @1.begin.line);}
    | ELGTE unary     {$$ = ast.make_elgte($2, @1.begin.line);}
    | SIZE unary      {$$ = ast.make_size($2, @1.begin.line);}
    | REDUCE unary '[' INTEGER ',' INTEGER ']' {$$ = ast.make_change<ast::Reduce>($2, $4, $6, @1.begin.line);}
    | EXTEND unary '[' INTEGER ',' INTEGER ']' {$$ = ast.make_change<ast::Extend>($2, $4, $6, @1.begin.line);}
    ;
primary
    : IDENTIFIER                {$$ = ast.make_ref($1, @1.begin.line);}
    | GET IDENTIFIER            {$$ = ast.make_res($2, @1.begin.line);}
    | GET_ENVIRONMENT           {$$ = ast.make_env(@1.begin.line);}
    | '(' rval ')'              {$$ = $2;}
    ;
