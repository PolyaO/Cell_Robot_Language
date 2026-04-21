
    | IDENTIFIER '=' INTEGER   {$$ = ast.make_val_assignement<int>($1, {}, $3, @1.begin.line);}
    | IDENTIFIER '[' dim_list ']' '=' INTEGER   {$$ = ast.make_val_assignement<int>($1, std::move($3), $6, @1.begin.line);}
    | IDENTIFIER '=' BOOLEAN   {$$ = ast.make_val_assignement<bool_t>($1, {}, $3, @1.begin.line);}
    | IDENTIFIER '[' dim_list ']' '=' BOOLEAN   {$$ = ast.make_val_assignement<bool_t>($1, std::move($3), $6, @1.begin.line);}
