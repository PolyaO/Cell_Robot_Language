// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.hpp"


// Unqualified %code blocks.
#line 23 "src/interpreter/parse.yy"

    #include "interpreter/ast_maker.hpp"

#line 50 "include/gen/parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 142 "include/gen/parser.cpp"

  /// Build a parser object.
  parser::parser (ast::AstMaker& ast_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      ast (ast_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_BOOLEAN: // "boolean"
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // "integer"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.YY_MOVE_OR_COPY< std::variant<unsigned, std::vector<unsigned>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_dim_list: // dim_list
        value.YY_MOVE_OR_COPY< std::vector<unsigned> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_task: // task
      case symbol_kind::S_any_task: // any_task
      case symbol_kind::S_find_exit: // find_exit
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_complex_expr_stmt: // complex_expr_stmt
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_rval: // rval
      case symbol_kind::S_unary: // unary
      case symbol_kind::S_primary: // primary
        value.YY_MOVE_OR_COPY< unsigned > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_BOOLEAN: // "boolean"
        value.move< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER: // "integer"
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.move< std::variant<unsigned, std::vector<unsigned>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_dim_list: // dim_list
        value.move< std::vector<unsigned> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_task: // task
      case symbol_kind::S_any_task: // any_task
      case symbol_kind::S_find_exit: // find_exit
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_complex_expr_stmt: // complex_expr_stmt
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_rval: // rval
      case symbol_kind::S_unary: // unary
      case symbol_kind::S_primary: // primary
        value.move< unsigned > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_BOOLEAN: // "boolean"
        value.copy< bool > (that.value);
        break;

      case symbol_kind::S_INTEGER: // "integer"
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.copy< std::variant<unsigned, std::vector<unsigned>> > (that.value);
        break;

      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_dim_list: // dim_list
        value.copy< std::vector<unsigned> > (that.value);
        break;

      case symbol_kind::S_task: // task
      case symbol_kind::S_any_task: // any_task
      case symbol_kind::S_find_exit: // find_exit
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_complex_expr_stmt: // complex_expr_stmt
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_rval: // rval
      case symbol_kind::S_unary: // unary
      case symbol_kind::S_primary: // primary
        value.copy< unsigned > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_BOOLEAN: // "boolean"
        value.move< bool > (that.value);
        break;

      case symbol_kind::S_INTEGER: // "integer"
        value.move< int > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.move< std::variant<unsigned, std::vector<unsigned>> > (that.value);
        break;

      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_dim_list: // dim_list
        value.move< std::vector<unsigned> > (that.value);
        break;

      case symbol_kind::S_task: // task
      case symbol_kind::S_any_task: // any_task
      case symbol_kind::S_find_exit: // find_exit
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_complex_expr_stmt: // complex_expr_stmt
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_rval: // rval
      case symbol_kind::S_unary: // unary
      case symbol_kind::S_primary: // primary
        value.move< unsigned > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // Discard the LAC context in case there still is one left from a
    // previous invocation.
    yy_lac_discard_ ("init");

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (ast));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;

        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    yy_lac_discard_ ("shift");
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_BOOLEAN: // "boolean"
        yylhs.value.emplace< bool > ();
        break;

      case symbol_kind::S_INTEGER: // "integer"
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_arg_list: // arg_list
        yylhs.value.emplace< std::variant<unsigned, std::vector<unsigned>> > ();
        break;

      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_dim_list: // dim_list
        yylhs.value.emplace< std::vector<unsigned> > ();
        break;

      case symbol_kind::S_task: // task
      case symbol_kind::S_any_task: // any_task
      case symbol_kind::S_find_exit: // find_exit
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_complex_expr_stmt: // complex_expr_stmt
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_rval: // rval
      case symbol_kind::S_unary: // unary
      case symbol_kind::S_primary: // primary
        yylhs.value.emplace< unsigned > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // programm: %empty
#line 51 "src/interpreter/parse.yy"
                                    {}
#line 712 "include/gen/parser.cpp"
    break;

  case 3: // programm: tasks
#line 52 "src/interpreter/parse.yy"
                                    {}
#line 718 "include/gen/parser.cpp"
    break;

  case 4: // tasks: task
#line 55 "src/interpreter/parse.yy"
                                       {}
#line 724 "include/gen/parser.cpp"
    break;

  case 5: // tasks: tasks task
#line 56 "src/interpreter/parse.yy"
                                       {}
#line 730 "include/gen/parser.cpp"
    break;

  case 6: // task: any_task
#line 59 "src/interpreter/parse.yy"
                                       {yylhs.value.as < unsigned > () = yystack_[0].value.as < unsigned > ();}
#line 736 "include/gen/parser.cpp"
    break;

  case 7: // task: find_exit
#line 60 "src/interpreter/parse.yy"
                                       {yylhs.value.as < unsigned > () = yystack_[0].value.as < unsigned > ();}
#line 742 "include/gen/parser.cpp"
    break;

  case 8: // any_task: TASK "identifier" arg_list '(' newline_opt stmts RESULT "identifier" '\n' ')' newline_opt
#line 63 "src/interpreter/parse.yy"
                                                                                             {yylhs.value.as < unsigned > () = ast.make_task(yystack_[9].value.as < std::string > (), yystack_[8].value.as < std::variant<unsigned, std::vector<unsigned>> > (), std::move(yystack_[5].value.as < std::vector<unsigned> > ()), yystack_[3].value.as < std::string > ());}
#line 748 "include/gen/parser.cpp"
    break;

  case 9: // find_exit: TASK FINDEXIT '(' newline_opt stmts ')' newline_opt
#line 66 "src/interpreter/parse.yy"
                                                        {yylhs.value.as < unsigned > () = ast.make_find_exit(std::move(yystack_[2].value.as < std::vector<unsigned> > ()));}
#line 754 "include/gen/parser.cpp"
    break;

  case 10: // expr: VAR "identifier" '=' "integer"
#line 69 "src/interpreter/parse.yy"
                                                     {yylhs.value.as < unsigned > () = ast.make_var<int>(yystack_[2].value.as < std::string > (), {}, yystack_[0].value.as < int > ());}
#line 760 "include/gen/parser.cpp"
    break;

  case 11: // expr: VAR "identifier" '=' "boolean"
#line 70 "src/interpreter/parse.yy"
                                                     {yylhs.value.as < unsigned > () = ast.make_var<bool>(yystack_[2].value.as < std::string > (), {}, yystack_[0].value.as < bool > ());}
#line 766 "include/gen/parser.cpp"
    break;

  case 12: // expr: VAR "identifier" '[' dim_list ']' '=' "integer"
#line 71 "src/interpreter/parse.yy"
                                                     {yylhs.value.as < unsigned > () = ast.make_var<int>(yystack_[5].value.as < std::string > (), yystack_[3].value.as < std::vector<unsigned> > (), yystack_[0].value.as < int > ());}
#line 772 "include/gen/parser.cpp"
    break;

  case 13: // expr: VAR "identifier" '[' dim_list ']' '=' "boolean"
#line 72 "src/interpreter/parse.yy"
                                                     {yylhs.value.as < unsigned > () = ast.make_var<bool>(yystack_[5].value.as < std::string > (), yystack_[3].value.as < std::vector<unsigned> > (), yystack_[0].value.as < bool > ());}
#line 778 "include/gen/parser.cpp"
    break;

  case 14: // expr: LOGITIZE "identifier"
#line 73 "src/interpreter/parse.yy"
                            {yylhs.value.as < unsigned > () = ast.make_logitize(yystack_[0].value.as < std::string > ());}
#line 784 "include/gen/parser.cpp"
    break;

  case 15: // expr: DIGITIZE "identifier"
#line 74 "src/interpreter/parse.yy"
                            {yylhs.value.as < unsigned > () = ast.make_digitize(yystack_[0].value.as < std::string > ());}
#line 790 "include/gen/parser.cpp"
    break;

  case 16: // expr: "identifier" '=' rval
#line 75 "src/interpreter/parse.yy"
                            {yylhs.value.as < unsigned > () = ast.make_assignement(yystack_[2].value.as < std::string > (), {}, yystack_[0].value.as < unsigned > ());}
#line 796 "include/gen/parser.cpp"
    break;

  case 17: // expr: "identifier" '[' dim_list ']' '=' rval
#line 76 "src/interpreter/parse.yy"
                                             {yylhs.value.as < unsigned > () = ast.make_assignement(yystack_[5].value.as < std::string > (), std::move(yystack_[3].value.as < std::vector<unsigned> > ()), yystack_[0].value.as < unsigned > ());}
#line 802 "include/gen/parser.cpp"
    break;

  case 18: // expr: MOVE
#line 77 "src/interpreter/parse.yy"
                            {yylhs.value.as < unsigned > () = ast.make_robot_expr<Move>();}
#line 808 "include/gen/parser.cpp"
    break;

  case 19: // expr: ROTATE_LEFT
#line 78 "src/interpreter/parse.yy"
                            {yylhs.value.as < unsigned > () = ast.make_robot_expr<RotateL>();}
#line 814 "include/gen/parser.cpp"
    break;

  case 20: // expr: ROTATE_RIGHT
#line 79 "src/interpreter/parse.yy"
                            {yylhs.value.as < unsigned > () = ast.make_robot_expr<RotateR>();}
#line 820 "include/gen/parser.cpp"
    break;

  case 21: // expr: DO "identifier" arg_list
#line 80 "src/interpreter/parse.yy"
                             {yylhs.value.as < unsigned > () = ast.make_do(yystack_[1].value.as < std::string > (), std::move(yystack_[0].value.as < std::variant<unsigned, std::vector<unsigned>> > ()));}
#line 826 "include/gen/parser.cpp"
    break;

  case 22: // complex_expr_stmt: FOR "identifier" BOUNDARY "identifier" STEP "identifier" newline_opt stmt
#line 83 "src/interpreter/parse.yy"
                                                                          {yylhs.value.as < unsigned > () = ast.make_for(yystack_[6].value.as < std::string > (), yystack_[4].value.as < std::string > (), yystack_[2].value.as < std::string > (), yystack_[0].value.as < unsigned > ());}
#line 832 "include/gen/parser.cpp"
    break;

  case 23: // complex_expr_stmt: SWITCH rval newline_opt "boolean" newline_opt stmt
#line 84 "src/interpreter/parse.yy"
                                                                                                       {yylhs.value.as < unsigned > () = ast.make_switch(yystack_[4].value.as < unsigned > (), yystack_[2].value.as < bool > (), yystack_[0].value.as < unsigned > (), false, 0);}
#line 838 "include/gen/parser.cpp"
    break;

  case 24: // complex_expr_stmt: SWITCH rval newline_opt "boolean" newline_opt stmt "boolean" newline_opt stmt
#line 85 "src/interpreter/parse.yy"
                                                                                                       {yylhs.value.as < unsigned > () = ast.make_switch(yystack_[7].value.as < unsigned > (), yystack_[5].value.as < bool > (), yystack_[3].value.as < unsigned > (), yystack_[2].value.as < bool > (), yystack_[0].value.as < unsigned > ());}
#line 844 "include/gen/parser.cpp"
    break;

  case 25: // complex_expr_stmt: '(' newline_opt stmts ')'
#line 86 "src/interpreter/parse.yy"
                                                              {yylhs.value.as < unsigned > () = ast.make_scope(std::move(yystack_[1].value.as < std::vector<unsigned> > ()));}
#line 850 "include/gen/parser.cpp"
    break;

  case 26: // stmt: PLEASE expr THANKS '\n' newline_opt
#line 89 "src/interpreter/parse.yy"
                                                          {ast.set_politely_asked(yystack_[3].value.as < unsigned > ()); yylhs.value.as < unsigned > () = yystack_[3].value.as < unsigned > ();}
#line 856 "include/gen/parser.cpp"
    break;

  case 27: // stmt: expr '\n' newline_opt
#line 90 "src/interpreter/parse.yy"
                                                          {yylhs.value.as < unsigned > () = yystack_[2].value.as < unsigned > ();}
#line 862 "include/gen/parser.cpp"
    break;

  case 28: // stmt: complex_expr_stmt newline_opt
#line 91 "src/interpreter/parse.yy"
                                                          {yylhs.value.as < unsigned > () = yystack_[1].value.as < unsigned > ();}
#line 868 "include/gen/parser.cpp"
    break;

  case 29: // stmts: stmt
#line 94 "src/interpreter/parse.yy"
                           {yylhs.value.as < std::vector<unsigned> > () = ast.make_stmts(); ast.add_to_stmts(yylhs.value.as < std::vector<unsigned> > (), yystack_[0].value.as < unsigned > ());}
#line 874 "include/gen/parser.cpp"
    break;

  case 30: // stmts: stmts stmt
#line 95 "src/interpreter/parse.yy"
                           {yylhs.value.as < std::vector<unsigned> > () = yystack_[1].value.as < std::vector<unsigned> > (); ast.add_to_stmts(yylhs.value.as < std::vector<unsigned> > (), yystack_[0].value.as < unsigned > ());}
#line 880 "include/gen/parser.cpp"
    break;

  case 33: // dim_list: "integer"
#line 102 "src/interpreter/parse.yy"
                           {yylhs.value.as < std::vector<unsigned> > () = ast.make_dim_list(); ast.add_to_dim_list(yylhs.value.as < std::vector<unsigned> > (), yystack_[0].value.as < int > ());}
#line 886 "include/gen/parser.cpp"
    break;

  case 34: // dim_list: dim_list ',' "integer"
#line 103 "src/interpreter/parse.yy"
                           {yylhs.value.as < std::vector<unsigned> > () = yystack_[2].value.as < std::vector<unsigned> > (); ast.add_to_dim_list(yystack_[2].value.as < std::vector<unsigned> > (), yystack_[0].value.as < int > ());}
#line 892 "include/gen/parser.cpp"
    break;

  case 35: // arg_list: "identifier"
#line 106 "src/interpreter/parse.yy"
                              {yylhs.value.as < std::variant<unsigned, std::vector<unsigned>> > () = ast.make_arg_list(); ast.add_to_arg_list(yylhs.value.as < std::variant<unsigned, std::vector<unsigned>> > (), yystack_[0].value.as < std::string > ());}
#line 898 "include/gen/parser.cpp"
    break;

  case 36: // arg_list: arg_list ',' "identifier"
#line 107 "src/interpreter/parse.yy"
                              {yylhs.value.as < std::variant<unsigned, std::vector<unsigned>> > () = yystack_[2].value.as < std::variant<unsigned, std::vector<unsigned>> > (); ast.add_to_arg_list(yystack_[2].value.as < std::variant<unsigned, std::vector<unsigned>> > (), yystack_[0].value.as < std::string > ());}
#line 904 "include/gen/parser.cpp"
    break;

  case 37: // rval: rval AND rval
#line 110 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_and(yystack_[2].value.as < unsigned > (), yystack_[0].value.as < unsigned > ());}
#line 910 "include/gen/parser.cpp"
    break;

  case 38: // rval: rval OR rval
#line 111 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_or(yystack_[2].value.as < unsigned > (), yystack_[0].value.as < unsigned > ());}
#line 916 "include/gen/parser.cpp"
    break;

  case 39: // rval: rval '+' rval
#line 112 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_sum(yystack_[2].value.as < unsigned > (), yystack_[0].value.as < unsigned > ());}
#line 922 "include/gen/parser.cpp"
    break;

  case 40: // rval: rval '-' rval
#line 113 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_sub(yystack_[2].value.as < unsigned > (), yystack_[0].value.as < unsigned > ());}
#line 928 "include/gen/parser.cpp"
    break;

  case 41: // rval: rval '*' rval
#line 114 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_mul(yystack_[2].value.as < unsigned > (), yystack_[0].value.as < unsigned > ());}
#line 934 "include/gen/parser.cpp"
    break;

  case 42: // rval: rval '/' rval
#line 115 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_div(yystack_[2].value.as < unsigned > (), yystack_[0].value.as < unsigned > ());}
#line 940 "include/gen/parser.cpp"
    break;

  case 43: // rval: rval '[' dim_list ']'
#line 116 "src/interpreter/parse.yy"
                             {yylhs.value.as < unsigned > () = ast.make_idx(yystack_[3].value.as < unsigned > (), std::move(yystack_[1].value.as < std::vector<unsigned> > ()));}
#line 946 "include/gen/parser.cpp"
    break;

  case 44: // rval: unary
#line 117 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = yystack_[0].value.as < unsigned > ();}
#line 952 "include/gen/parser.cpp"
    break;

  case 45: // unary: primary
#line 120 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = yystack_[0].value.as < unsigned > ();}
#line 958 "include/gen/parser.cpp"
    break;

  case 46: // unary: NOT unary
#line 121 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_not(yystack_[0].value.as < unsigned > ());}
#line 964 "include/gen/parser.cpp"
    break;

  case 47: // unary: MXTRUE unary
#line 122 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_mxtrue(yystack_[0].value.as < unsigned > ());}
#line 970 "include/gen/parser.cpp"
    break;

  case 48: // unary: MXFALSE unary
#line 123 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_mxfalse(yystack_[0].value.as < unsigned > ());}
#line 976 "include/gen/parser.cpp"
    break;

  case 49: // unary: MXEQ unary
#line 124 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_mxeq(yystack_[0].value.as < unsigned > ());}
#line 982 "include/gen/parser.cpp"
    break;

  case 50: // unary: MXLT unary
#line 125 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_mxlt(yystack_[0].value.as < unsigned > ());}
#line 988 "include/gen/parser.cpp"
    break;

  case 51: // unary: MXGT unary
#line 126 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_mxgt(yystack_[0].value.as < unsigned > ());}
#line 994 "include/gen/parser.cpp"
    break;

  case 52: // unary: MXLTE unary
#line 127 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_mxlte(yystack_[0].value.as < unsigned > ());}
#line 1000 "include/gen/parser.cpp"
    break;

  case 53: // unary: MXGTE unary
#line 128 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_mxgte(yystack_[0].value.as < unsigned > ());}
#line 1006 "include/gen/parser.cpp"
    break;

  case 54: // unary: ELEQ unary
#line 129 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_eleq(yystack_[0].value.as < unsigned > ());}
#line 1012 "include/gen/parser.cpp"
    break;

  case 55: // unary: ELLT unary
#line 130 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_ellt(yystack_[0].value.as < unsigned > ());}
#line 1018 "include/gen/parser.cpp"
    break;

  case 56: // unary: ELGT unary
#line 131 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_elgt(yystack_[0].value.as < unsigned > ());}
#line 1024 "include/gen/parser.cpp"
    break;

  case 57: // unary: ELLTE unary
#line 132 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_ellte(yystack_[0].value.as < unsigned > ());}
#line 1030 "include/gen/parser.cpp"
    break;

  case 58: // unary: ELGTE unary
#line 133 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_elgte(yystack_[0].value.as < unsigned > ());}
#line 1036 "include/gen/parser.cpp"
    break;

  case 59: // unary: SIZE unary
#line 134 "src/interpreter/parse.yy"
                      {yylhs.value.as < unsigned > () = ast.make_size(yystack_[0].value.as < unsigned > ());}
#line 1042 "include/gen/parser.cpp"
    break;

  case 60: // unary: REDUCE unary '[' "integer" ',' "integer" ']'
#line 135 "src/interpreter/parse.yy"
                                               {yylhs.value.as < unsigned > () = ast.make_reduce(yystack_[5].value.as < unsigned > (), yystack_[3].value.as < int > (), yystack_[1].value.as < int > ());}
#line 1048 "include/gen/parser.cpp"
    break;

  case 61: // unary: EXTEND unary '[' "integer" ',' "integer" ']'
#line 136 "src/interpreter/parse.yy"
                                               {yylhs.value.as < unsigned > () = ast.make_extend(yystack_[5].value.as < unsigned > (), yystack_[3].value.as < int > (), yystack_[1].value.as < int > ());}
#line 1054 "include/gen/parser.cpp"
    break;

  case 62: // primary: "identifier"
#line 139 "src/interpreter/parse.yy"
                                {yylhs.value.as < unsigned > () = ast.make_ref(yystack_[0].value.as < std::string > ());}
#line 1060 "include/gen/parser.cpp"
    break;

  case 63: // primary: GET "identifier"
#line 140 "src/interpreter/parse.yy"
                                {yylhs.value.as < unsigned > () = ast.make_res(yystack_[0].value.as < std::string > ());}
#line 1066 "include/gen/parser.cpp"
    break;

  case 64: // primary: GET_ENVIRONMENT
#line 141 "src/interpreter/parse.yy"
                                {yylhs.value.as < unsigned > () = ast.make_env();}
#line 1072 "include/gen/parser.cpp"
    break;

  case 65: // primary: '(' rval ')'
#line 142 "src/interpreter/parse.yy"
                                {yylhs.value.as < unsigned > () = yystack_[1].value.as < unsigned > ();}
#line 1078 "include/gen/parser.cpp"
    break;


#line 1082 "include/gen/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      yy_lac_discard_ ("error recovery");
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "LOWEST", "identifier",
  "integer", "boolean", "TASK", "FINDEXIT", "RESULT", "DO", "GET", "FOR",
  "BOUNDARY", "STEP", "SWITCH", "VAR", "SIZE", "LOGITIZE", "DIGITIZE",
  "REDUCE", "EXTEND", "MOVE", "ROTATE_LEFT", "ROTATE_RIGHT",
  "GET_ENVIRONMENT", "PLEASE", "THANKS", "OR", "AND", "'+'", "'-'", "'*'",
  "'/'", "NOT", "MXTRUE", "MXFALSE", "MXEQ", "MXLT", "MXGT", "MXLTE",
  "MXGTE", "ELEQ", "ELLT", "ELGT", "ELLTE", "ELGTE", "'='", "'['", "']'",
  "'('", "')'", "'\\n'", "SWITCH_NO_TAIL", "','", "$accept", "programm",
  "tasks", "task", "any_task", "find_exit", "expr", "complex_expr_stmt",
  "stmt", "stmts", "newline_opt", "dim_list", "arg_list", "rval", "unary",
  "primary", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

#if YYDEBUG
    // Execute LAC once. We don't care if it is successful, we
    // only do it for the sake of debugging output.
    if (!yyparser_.yy_lac_established_)
      yyparser_.yy_lac_check_ (yyla_.kind ());
#endif

    for (int yyx = 0; yyx < YYNTOKENS; ++yyx)
      {
        symbol_kind_type yysym = YY_CAST (symbol_kind_type, yyx);
        if (yysym != symbol_kind::S_YYerror
            && yysym != symbol_kind::S_YYUNDEF
            && yyparser_.yy_lac_check_ (yysym))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
      }
    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }




  bool
  parser::yy_lac_check_ (symbol_kind_type yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if YYDEBUG
    YYCDEBUG << "LAC: checking lookahead " << symbol_name (yytoken) << ':';
#endif
    std::ptrdiff_t lac_top = 0;
    while (true)
      {
        state_type top_state = (yylac_stack_.empty ()
                                ? yystack_[lac_top].state
                                : yylac_stack_.back ());
        int yyrule = yypact_[+top_state];
        if (yy_pact_value_is_default_ (yyrule)
            || (yyrule += yytoken) < 0 || yylast_ < yyrule
            || yycheck_[yyrule] != yytoken)
          {
            // Use the default action.
            yyrule = yydefact_[+top_state];
            if (yyrule == 0)
              {
                YYCDEBUG << " Err\n";
                return false;
              }
          }
        else
          {
            // Use the action from yytable.
            yyrule = yytable_[yyrule];
            if (yy_table_value_is_error_ (yyrule))
              {
                YYCDEBUG << " Err\n";
                return false;
              }
            if (0 < yyrule)
              {
                YYCDEBUG << " S" << yyrule << '\n';
                return true;
              }
            yyrule = -yyrule;
          }
        // By now we know we have to simulate a reduce.
        YYCDEBUG << " R" << yyrule - 1;
        // Pop the corresponding number of values from the stack.
        {
          std::ptrdiff_t yylen = yyr2_[yyrule];
          // First pop from the LAC stack as many tokens as possible.
          std::ptrdiff_t lac_size = std::ptrdiff_t (yylac_stack_.size ());
          if (yylen < lac_size)
            {
              yylac_stack_.resize (std::size_t (lac_size - yylen));
              yylen = 0;
            }
          else if (lac_size)
            {
              yylac_stack_.clear ();
              yylen -= lac_size;
            }
          // Only afterwards look at the main stack.
          // We simulate popping elements by incrementing lac_top.
          lac_top += yylen;
        }
        // Keep top_state in sync with the updated stack.
        top_state = (yylac_stack_.empty ()
                     ? yystack_[lac_top].state
                     : yylac_stack_.back ());
        // Push the resulting state of the reduction.
        state_type state = yy_lr_goto_state_ (top_state, yyr1_[yyrule]);
        YYCDEBUG << " G" << int (state);
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  parser::yy_lac_establish_ (symbol_kind_type yytoken)
  {
    /* Establish the initial context for the current lookahead if no initial
       context is currently established.

       We define a context as a snapshot of the parser stacks.  We define
       the initial context for a lookahead as the context in which the
       parser initially examines that lookahead in order to select a
       syntactic action.  Thus, if the lookahead eventually proves
       syntactically unacceptable (possibly in a later context reached via a
       series of reductions), the initial context can be used to determine
       the exact set of tokens that would be syntactically acceptable in the
       lookahead's place.  Moreover, it is the context after which any
       further semantic actions would be erroneous because they would be
       determined by a syntactically unacceptable token.

       yy_lac_establish_ should be invoked when a reduction is about to be
       performed in an inconsistent state (which, for the purposes of LAC,
       includes consistent states that don't know they're consistent because
       their default reductions have been disabled).

       For parse.lac=full, the implementation of yy_lac_establish_ is as
       follows.  If no initial context is currently established for the
       current lookahead, then check if that lookahead can eventually be
       shifted if syntactic actions continue from the current context.  */
    if (yy_lac_established_)
      return true;
    else
      {
#if YYDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << symbol_name (yytoken) << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
  }

  // Discard any previous initial lookahead context.
  void
  parser::yy_lac_discard_ (const char* event)
  {
   /* Discard any previous initial lookahead context because of Event,
      which may be a lookahead change or an invalidation of the currently
      established initial context for the current lookahead.

      The most common example of a lookahead change is a shift.  An example
      of both cases is syntax error recovery.  That is, a syntax error
      occurs when the lookahead is syntactically erroneous for the
      currently established initial context, so error recovery manipulates
      the parser stacks to try to find a new initial context in which the
      current lookahead is syntactically acceptable.  If it fails to find
      such a context, it discards the lookahead.  */
    if (yy_lac_established_)
      {
        YYCDEBUG << "LAC: initial context discarded due to "
                 << event << '\n';
        yy_lac_established_ = false;
      }
  }


  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
         In the first two cases, it might appear that the current syntax
         error should have been detected in the previous state when
         yy_lac_check was invoked.  However, at that time, there might
         have been a different syntax error that discarded a different
         initial context during error recovery, leaving behind the
         current lookahead.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -90;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
      19,    30,    37,    19,   -90,   -90,   -90,    54,     9,   -90,
     -90,   -90,    -8,   -90,   -90,    56,    17,    17,   -90,    -3,
      64,    66,   139,    68,    69,    70,   -90,   -90,   -90,    84,
     -90,   -90,    24,   -90,   -90,    67,   182,   139,    73,    54,
      71,   -90,    76,   139,   139,   139,   -90,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   181,   -90,   -90,     1,   -90,   -90,    60,    17,   -90,
      29,   -90,   -90,    88,   181,   -90,   -34,    41,    93,   -90,
     -90,    50,    51,   -90,   -90,   -90,   -90,   -90,   -90,   -90,
     -90,   -90,   -90,   -90,   -90,   -90,   -20,   139,   139,   139,
     139,   139,   139,    73,    -2,    46,    73,    49,   118,    29,
      29,    52,    62,   105,    98,   108,   109,   -90,   122,   115,
     -26,   -26,    72,    72,   -31,   -90,   -90,   -90,   -24,   -90,
     -90,    75,   139,   -90,   117,    65,    77,   -90,    17,    85,
      29,   -90,   181,   -90,   119,   130,   133,    48,    29,    17,
      74,   100,   -90,   -90,   -90,   -90,   -90,   -90,    17,   -90
  };

  const signed char
  parser::yydefact_[] =
  {
       2,     0,     0,     3,     4,     6,     7,     0,     0,     1,
       5,    35,     0,    31,    31,     0,     0,     0,    36,     0,
       0,     0,     0,     0,     0,     0,    18,    19,    20,     0,
      31,    32,     0,    31,    29,     0,     0,     0,     0,     0,
       0,    62,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    44,    45,     0,    14,    15,     0,     0,    31,
      28,    31,    30,     0,    16,    33,     0,    21,     0,    63,
      59,     0,     0,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       9,     0,     0,     0,     0,     0,     0,    65,    38,    37,
      39,    40,    41,    42,     0,    31,    10,    11,     0,    31,
      25,     0,     0,    34,     0,     0,     0,    43,     0,     0,
      26,    31,    17,    31,     0,     0,    23,     0,     8,     0,
       0,     0,    31,    12,    13,    22,    60,    61,     0,    24
  };

  const short
  parser::yypgoto_[] =
  {
     -90,   -90,   -90,   154,   -90,   -90,   129,   -90,   -33,   -12,
     -14,   -89,   123,   -36,   190,   -90
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     2,     3,     4,     5,     6,    32,    33,    34,    35,
      16,    76,    12,    61,    62,    63
  };

  const unsigned char
  parser::yytable_[] =
  {
      17,    74,    72,    72,   125,    36,   101,   102,    97,    98,
      99,   100,   101,   102,   124,   112,    68,   128,   137,    70,
     113,    19,   103,   113,    96,   139,     1,    20,   103,    21,
     113,   117,    22,    23,     7,    24,    25,     9,     8,    26,
      27,    28,    14,    29,    37,    38,    15,   104,   105,   106,
      31,   126,   127,   153,   154,   109,   108,   110,    11,    13,
      18,   118,   119,   120,   121,   122,   123,    30,    39,    31,
      40,    19,    64,    65,    66,    72,    69,    20,    75,    21,
      79,    31,    22,    23,    78,    24,    25,   107,    19,    26,
      27,    28,   111,    29,    20,    15,   142,   114,   115,   116,
      23,   129,    24,    25,   131,   146,    26,    27,    28,   132,
     133,   138,   134,   135,   136,   140,   155,    30,    71,   144,
     103,   143,    19,   156,   150,   159,   141,   148,    20,   149,
      21,   145,   147,    22,    23,   151,    24,    25,   158,   152,
      26,    27,    28,    41,    29,    99,   100,   101,   102,   157,
      42,    98,    99,   100,   101,   102,    43,    10,    67,    44,
      45,     0,    77,   103,    46,     0,     0,     0,    30,   130,
     103,     0,     0,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    19,     0,     0,    60,
       0,    73,    20,     0,    21,     0,     0,    22,    23,     0,
      24,    25,     0,     0,    26,    27,    28,     0,    29,    97,
      98,    99,   100,   101,   102,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
       0,     0,    30,    80,    81,    82,     0,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95
  };

  const short
  parser::yycheck_[] =
  {
      14,    37,    35,    36,     6,    17,    32,    33,    28,    29,
      30,    31,    32,    33,   103,    49,    30,   106,    49,    33,
      54,     4,    48,    54,    60,    49,     7,    10,    48,    12,
      54,    51,    15,    16,     4,    18,    19,     0,     8,    22,
      23,    24,    50,    26,    47,    48,    54,    61,    47,    48,
      52,     5,     6,     5,     6,    69,    68,    71,     4,    50,
       4,    97,    98,    99,   100,   101,   102,    50,     4,    52,
       4,     4,     4,     4,     4,   108,    52,    10,     5,    12,
       4,    52,    15,    16,    13,    18,    19,    27,     4,    22,
      23,    24,     4,    26,    10,    54,   132,     4,    48,    48,
      16,    52,    18,    19,    52,   138,    22,    23,    24,    47,
       5,   125,    14,     5,     5,   129,   149,    50,    51,    54,
      48,     4,     4,    49,     5,   158,    51,   141,    10,   143,
      12,    54,    47,    15,    16,     5,    18,    19,   152,     6,
      22,    23,    24,     4,    26,    30,    31,    32,    33,    49,
      11,    29,    30,    31,    32,    33,    17,     3,    29,    20,
      21,    -1,    39,    48,    25,    -1,    -1,    -1,    50,    51,
      48,    -1,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,     4,    -1,    -1,    50,
      -1,     9,    10,    -1,    12,    -1,    -1,    15,    16,    -1,
      18,    19,    -1,    -1,    22,    23,    24,    -1,    26,    28,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    50,    43,    44,    45,    -1,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59
  };

  const signed char
  parser::yystos_[] =
  {
       0,     7,    56,    57,    58,    59,    60,     4,     8,     0,
      58,     4,    67,    50,    50,    54,    65,    65,     4,     4,
      10,    12,    15,    16,    18,    19,    22,    23,    24,    26,
      50,    52,    61,    62,    63,    64,    64,    47,    48,     4,
       4,     4,    11,    17,    20,    21,    25,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      50,    68,    69,    70,     4,     4,     4,    61,    65,    52,
      65,    51,    63,     9,    68,     5,    66,    67,    13,     4,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    68,    28,    29,    30,
      31,    32,    33,    48,    65,    47,    48,    27,    64,    65,
      65,     4,    49,    54,     4,    48,    48,    51,    68,    68,
      68,    68,    68,    68,    66,     6,     5,     6,    66,    52,
      51,    52,    47,     5,    14,     5,     5,    49,    65,    49,
      65,    51,    68,     4,    54,    54,    63,    47,    65,    65,
       5,     5,     6,     5,     6,    63,    49,    49,    65,    63
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    55,    56,    56,    57,    57,    58,    58,    59,    60,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    62,    62,    62,    62,    63,    63,    63,    64,
      64,    65,    65,    66,    66,    67,    67,    68,    68,    68,
      68,    68,    68,    68,    68,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    70,    70,    70,    70
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     1,     2,     1,     1,    11,     7,
       4,     4,     7,     7,     2,     2,     3,     6,     1,     1,
       1,     3,     8,     6,     9,     4,     5,     3,     2,     1,
       2,     0,     2,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     4,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       7,     7,     1,     2,     1,     3
  };




#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,    51,    51,    52,    55,    56,    59,    60,    63,    66,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    83,    84,    85,    86,    89,    90,    91,    94,
      95,    98,    99,   102,   103,   106,   107,   110,   111,   112,
     113,   114,   115,   116,   117,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   139,   140,   141,   142
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1755 "include/gen/parser.cpp"

