%{

#include <string>
#include <cerrno>
#include <cstring>
#include "ast_maker.hpp"
#include "parser.hpp"

#define YY_USER_ACTION  loc.columns(yyleng);

using par = yy::parser;

par::symbol_type make_INTEGER(const std::string& s, int base,
                              const par::location_type& loc);
%}

%option noyywrap nounput noinput batch yylineno

OCTAL       0[0-7]*
DECIMAL     [1-9][0-9]*
HEXADECIMAL (0x[[:xdigit:]]+)|({DECIMAL}+[:xdigit:]*)
IDENTIFIER  [a-zA-Z_][a-zA-Z0-9_]*
SIGN        []+*=/,()[-]
BLANC       [ \t\r]

%%

%{
  yy::location &loc = ast.loc;
  loc.step ();
%}

"TASK"             return par::make_TASK(loc);
"DO"               return par::make_DO(loc);
"GET"              return par::make_GET(loc);
"RESULT"           return par::make_RESULT(loc);

"FOR"              return par::make_FOR(loc);
"BOUNDARY"         return par::make_BOUNDARY(loc);
"STEP"             return par::make_STEP(loc);

"SWITCH"           return par::make_SWITCH(loc);

"VAR"              return par::make_VAR(loc);
"SIZE"             return par::make_SIZE(loc);
"LOGITIZE"         return par::make_LOGITIZE(loc);
"DIGITIZE"         return par::make_DIGITIZE(loc);
"REDUCE"           return par::make_REDUCE(loc);
"EXTEND"           return par::make_EXTEND(loc);

"MXEQ"             return par::make_MXEQ(loc);
"MXLT"             return par::make_MXLT(loc);
"MXGT"             return par::make_MXGT(loc);
"MXLTE"            return par::make_MXLTE(loc);
"MXGTE"            return par::make_MXGTE(loc);

"ELEQ"             return par::make_ELEQ(loc);
"ELLT"             return par::make_ELLT(loc);
"ELGT"             return par::make_ELGT(loc);
"ELLTE"            return par::make_ELLTE(loc);
"ELGTE"            return par::make_ELGTE(loc);

"AND"              return par::make_AND(loc);
"OR"              return par::make_OR(loc);

"MXTRUE"           return par::make_MXTRUE(loc);
"MXFALSE"          return par::make_MXFALSE(loc);

"MOVE"             return par::make_MOVE(loc);
"ROTATE LEFT"      return par::make_ROTATE_LEFT(loc);
"ROTATE RIGHT"     return par::make_ROTATE_RIGHT(loc);
"GET ENVIRONMENT"  return par::make_GET_ENVIRONMENT(loc);

"PLEASE"           return par::make_PLEASE(loc);
"THANKS"           return par::make_THANKS(loc);

"TRUE"             return par::make_BOOLEAN(true, loc);
"FALSE"            return par::make_BOOLEAN(false, loc);

-?{OCTAL}          return make_INTEGER(yytext, 8, loc);
-?{DECIMAL}        return make_INTEGER(yytext, 10, loc);
-?{HEXADECIMAL}    return make_INTEGER(yytext, 16, loc);

{IDENTIFIER}       return par::make_IDENTIFIER(yytext, loc);
{SIGN}             return par::symbol_type(*yytext, loc);
{BLANC}+           loc.step();
\n                 loc.lines(yyleng); loc.step(); return par::symbol_type(*yytext, loc);
.                  { throw par::syntax_error
                        (loc, "Flex: invalid character: " + std::string(yytext));
}
<<EOF>>            return par::make_YYEOF(loc);


%%

par::symbol_type make_INTEGER(const std::string &s, int base,
                              const par::location_type &loc) {
    int n = 0;
    try {
        n = std::stoi(s, nullptr, base);
    } catch (std::out_of_range const &ex) {
        throw par::syntax_error(loc, "Flex: integer is out of range: " + s);
    }
    return par::make_INTEGER(n, loc);
}


