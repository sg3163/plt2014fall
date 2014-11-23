%{ open Ast %}

%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK COMMA SEMI 
%token PLUS MINUS TIMES DIVIDE ASSIGN ACCESS COMPLUS COMMINUS
%token EQ NEQ LT GT LEQ GEQ NOT MOD
%token IF THEN ELSE ELIF
%token AND OR FOR IN
%token FUNC END DECL
%token NOTIN READ PRINT TYPE TYPESTRUCT JOIN MAKESTRING RETURN
%token <int> NUM_LIT
%token <string> STRING_LIT
%token <string> JSON_LIT
%token <string> LIST_LIT
%token <string> BOOL_LIT
%token <string> ID
%token EOF

%right ASSIGN NOT

%left AND OR
%left EQ NEQ
%left LT GT LEQ GEQ
%left PLUS MINUS 
%left TIMES DIVIDE
%left COMPLUS COMMINUS

%start program
%type <Ast.program> program

%%

program:
    { [] , [] }
    | program vdecl { ($2 :: fst $1) , snd $1 }
    | program fdecl { print_string "--fdecl\n"; fst $1, ($2 :: snd $1) } 

fdecl:
    FUNC ID LPAREN formals_opt RPAREN vdecl_opt stmt_list END
       {{
        return = Ast.StrType;
        fname = $2;
        formals = $4;
        fnlocals = List.rev $6;
        body = List.rev $7 }}

formals_opt:
    { [] }
    | formal_list   { List.rev $1 }

formal_list:
    formal                     { [$1] }
    | formal_list COMMA formal { $3 :: $1 }

formal:
    ID      { { vtype = StrType;  vname = $1; vexpr = NoExpr; } }
    
/* Var declarations can also be optional */
vdecl_opt:
    { [] }
    | vdecl_list    { List.rev $1 }

vdecl_list:
    vdecl              { [$1] }
    | vdecl_list vdecl { $2 :: $1 }

vdecl:
     DECL ID ASSIGN expr SEMI  { { vtype = StrType ;  vname = $2; vexpr = $4 } }

stmt_list:
    { [] }
    | stmt_list stmt { $2 :: $1 }

/* using SEMI to separate stmts for now */
stmt:
    expr SEMI                                           { Expr($1) }
 /*   | RETURN expr_opt SEMI                              { Return($2) } 
     | IF LPAREN expr RPAREN THEN stmt %prec NOELSE      { If($3, $6, Block([])) }
    | IF LPAREN expr RPAREN THEN stmt ELSE stmt         { If($3, $6, $8) }
    | PRINT expr SEMI                                   { Print($2) }
    | WHILE LPAREN expr RPAREN stmt                     { While($3, $5) } 
    | FOR LPAREN for_expr IN for_expr RPAREN stmt       { For($3, $5, $7 ) } 
    | IF list_expr IN list_expr THEN stmt %prec NOELSE  { Ifin($2, $4, $6, Block([])) }
    | IF list_expr IN list_expr THEN stmt ELSE stmt     { Ifin($2, $4, $6, $8) } 
    | LBRACE rev_stmt_list RBRACE                       { Block($2) } */


expr:
    | NUM_LIT                      { LitInt($1) }
    | STRING_LIT                   { LitStr($1) }
	| JSON_LIT                     { LitJson($1) }
	| LIST_LIT                     { LitList($1) } 
	| BOOL_LIT                     { LitBool($1) }
    | ID                           { Id($1) }
    | expr PLUS   expr             { Binop($1, Add,      $3) }
    | expr MINUS  expr             { Binop($1, Sub,      $3) }
    | expr TIMES  expr             { Binop($1, Mult,     $3) }
    | expr DIVIDE expr             { Binop($1, Div,      $3) }
    | expr EQ     expr             { Binop($1, Equal,    $3) }
    | expr NEQ    expr             { Binop($1, Neq,      $3) }
    | ID ASSIGN expr               { Assign($1, $3) }
