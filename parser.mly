%{ open Ast %}

%token LPAREN RPAREN LBRACE RBRACE SEMI
%token PLUS MINUS TIMES DIVIDE ASSIGN EOF
%token EQ NEQ PRINT LT GT LEQ GEQ
%token LBRACK RBRACK AND OR NOT
%token COMMINUS COMPLUS MODULUS IF THEN ELSE DOT
%token FOR IN END NOTIN READ TYPE TYPESTRUCT JOIN
%token MAKESTRING 
%token <int> LIT_INT
%token <string> LIT_STR
%token <string> LIT_JSON
%token <string> LIT_LIST
%token <string> ID

%right ASSIGN

%left EQ NEQ
%left PLUS MINUS
%left TIMES DIVIDE

%start program
%type <Ast.program> program

%%

program:
    { [] }
    | program stmt { $2 :: $1  }
/*    | program fdecl { fst $1, ($2 :: snd $1) } */

/* Using SEMI to separate variable declarations for now */
stmt:
      ID SEMI    { Assign(Id($1))}
    | ID ASSIGN expr SEMI  { Assign(Id($1), $3) }


/*stmt_list:
    { [] }
    | stmt_list stmt { $2 :: $1 }*/

rev_stmt_list:
    program          { List.rev $1 }

/* using SEMI to separate stmts for now */
stmt:
    expr SEMI                                           { Expr($1) }
/*  | RETURN expr_opt SEMI                              { Return($2) } 
    | IF LPAREN expr RPAREN THEN stmt %prec NOELSE      { If($3, $6, Block([])) }
    | IF LPAREN expr RPAREN THEN stmt ELSE stmt         { If($3, $6, $8) }*/
    | PRINT LPAREN expr RPAREN SEMI                          { Print($3) }
/*    | WHILE LPAREN expr RPAREN stmt                     { While($3, $5) } 
    | FOR LPAREN for_expr IN for_expr RPAREN stmt       { For($3, $5, $7 ) } 
    | IF list_expr IN list_expr THEN stmt %prec NOELSE  { Ifin($2, $4, $6, Block([])) }
    | IF list_expr IN list_expr THEN stmt ELSE stmt     { Ifin($2, $4, $6, $8) } */
    | LBRACE rev_stmt_list RBRACE                       { Block($2) }

expr:
    | LIT_INT                      { LitInt($1) }
    | LIT_STR                      { LitStr($1) }
		| LIT_JSON                     { LitJson($1) }
		| LIT_LIST                     { LitList($1) }
    | ID                           { Id($1) }
    | expr PLUS   expr             { Binop($1, Add,      $3) }
    | expr MINUS  expr             { Binop($1, Sub,      $3) }
    | expr TIMES  expr             { Binop($1, Mult,     $3) }
    | expr DIVIDE expr             { Binop($1, Div,      $3) }
    | expr EQ     expr             { Binop($1, Equal,    $3) }
    | expr NEQ    expr             { Binop($1, Neq,      $3) }
    | ID ASSIGN expr               { Assign($1, $3) }
    | LPAREN expr RPAREN           {$2}
