%{ open Ast %}

%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK COMMA SEMI 
%token PLUS MINUS TIMES DIVIDE ASSIGN ACCESS COMPLUS COMMINUS
%token EQ NEQ LT GT LEQ GEQ NOT MOD
%token RETURN IF THEN ELSE
%token AND OR FOR IN
%token FUNC END DECL MAINFUNC
%token NOTIN READ PRINT TYPE TYPESTRUCT JOIN MAKESTRING
%token <int> NUM_LIT
%token <string> STRING_LIT
%token <string> JSON_LIT
%token <string> LIST_LIT
%token <string> BOOL_LIT
%token <string> ID
%token EOF

%nonassoc NOELSE
%nonassoc ELSE

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
    | program fdecl { fst $1, ($2 :: snd $1) } 

fdecl:
    FUNC ID LPAREN formals_opt RPAREN LBRACE vdecl_opt stmt_list RBRACE
       {{
        return = Ast.StrType;
        fname = $2;
        formals = $4;
        fnlocals = List.rev $7;
        body = List.rev $8 }}

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
    | vdecl_list    { $1 }

vdecl_list:
    vdecl              { [$1] }
    | vdecl_list vdecl { $2 :: $1 }

vdecl:
     DECL ID ASSIGN expr SEMI  { { vtype = StrType ;  vname = $2; vexpr = $4 } }

stmt_list:
    { [] }
    | stmt_list stmt { $2 :: $1 }

rev_stmt_list:
    stmt_list          { List.rev $1 }

/* using SEMI to separate stmts for now */
stmt:
    expr SEMI                                           { Expr($1) }
    | RETURN expr_opt SEMI                              { Return($2) } 
	| PRINT expr SEMI                                   { Print($2) }
    | IF LPAREN expr RPAREN stmt ELSE stmt              { If($3, $5, $7) }
    | IF LPAREN expr RPAREN stmt %prec NOELSE           { If($3, $5, Block([])) }
    | LBRACE rev_stmt_list RBRACE                       { Block($2) }

expr_opt:
    /* nothing */ { NoExpr }
  | expr          { $1 }

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
	| MAINFUNC                     { MainRet(0) }

actuals_opt:
    /* nothing */   { [] }
    | actuals_list  { List.rev $1 }

actuals_list:
    expr                      { [$1] }
    | actuals_list COMMA expr { $3 :: $1 }