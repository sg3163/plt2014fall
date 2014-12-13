%{ open Ast %}

%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK COMMA SEMI 
%token PLUS MINUS TIMES DIVIDE ASSIGN ACCESS COMPLUS COMMINUS COLON
%token EQ NEQ LT GT LEQ GEQ NOT MOD
%token RETURN IF THEN ELSE HASH NULL
%token AND OR FOR IN
%token FUNC END DECL MAINFUNC
%token NOTIN READ PRINT TYPE TYPESTRUCT JOIN MAKESTRING ATTRLIST WRITE
%token <int> NUM_LIT
%token <string> STRING_LIT
%token <string> JSON_LIT
%token <string> LIST_LIT
%token <string> BOOL_LIT
%token <string> ID
%token EOF

%nonassoc NOELSE
%nonassoc ELSE

%right ASSIGN

%left AND OR
%left EQ NEQ
%left LT GT LEQ GEQ MOD
%left PLUS MINUS
%left TIMES DIVIDE
%left COMPLUS COMMINUS
%right NOT

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
    | PRINT LPAREN expr RPAREN SEMI                     { Print($3) }
	| TYPE LPAREN expr RPAREN SEMI										  { ObjType($3) }
	| FOR loop_var IN for_expr stmt       { For($2, $4, $5 ) } 
    | IF LPAREN expr RPAREN stmt ELSE stmt              { If($3, $5, $7) }
    | IF LPAREN expr RPAREN stmt %prec NOELSE           { If($3, $5, Block([])) }
    | IF LPAREN ID IN ID RPAREN stmt ELSE stmt      { Ifin($3, $5, $7, $9) }
    | IF LPAREN ID IN ID RPAREN stmt %prec NOELSE   { Ifin($3, $5, $7,Block([])) }
    | LBRACE rev_stmt_list RBRACE                       { Block($2) }
	| WRITE LPAREN expr COMMA STRING_LIT RPAREN SEMI		{ Write($3, $5) }


for_expr:
    ID                              { Forid($1) }
loop_var:
    ID                              { LoopVar($1) }

expr_opt:
    /* nothing */ { NoExpr }
  | expr          { $1 }

list_expr:
        NUM_LIT                      { ListItemInt($1) }
    | STRING_LIT                      { ListItemStr($1) }

expr:
    | NUM_LIT                      { LitInt($1) }
    | STRING_LIT                   { LitStr($1) }
    | HASH LBRACE json_items RBRACE HASH       { LitJson($3) }
    | LBRACK list_items RBRACK         { LitList($2) } 
    | BOOL_LIT                     { LitBool($1) }
	| NULL												 { LitNull("null") }
    | ID                           { Id($1) }
    | NOT LPAREN expr  RPAREN             {Not($3)}
    | NOT expr                      {Not($2)}
    | expr COMPLUS    expr        { Binop($1, Concat,  $3) }
    | expr COMMINUS    expr        { Binop($1, Minus,   $3) }
    | expr PLUS   expr             { Binop($1, Add,      $3) }
    | expr MINUS  expr             { Binop($1, Sub,      $3) }
    | expr TIMES  expr             { Binop($1, Mult,     $3) }
    | expr DIVIDE expr             { Binop($1, Div,      $3) }
    | expr EQ     expr             { Binop($1, Equal,    $3) }
    | expr NEQ    expr             { Binop($1, Neq,      $3) }
    | expr LT     expr             { Binop($1, Less,   $3) }
    | expr LEQ    expr             { Binop($1, Leq,   $3) }
    | expr GT     expr             { Binop($1, Greater,   $3) }
    | expr GEQ    expr             { Binop($1, Geq,   $3) }
    | expr MOD    expr             { Binop($1, Mod,   $3) }
    | expr AND expr                { Binop($1, And,      $3) }
    | expr OR expr                 { Binop($1, Or,       $3) }
    | ID ASSIGN   expr             { Assign($1, $3) }
    | ID LPAREN actuals_opt RPAREN { Call($1,   $3) }
    | MAINFUNC                     { MainRet(0) }
    | ID LBRACK list_expr RBRACK   { ElemAccess($1, $3) }
    | ID ACCESS TYPESTRUCT LPAREN   RPAREN   { TypeStruct($1) }
    | ID ACCESS ATTRLIST LPAREN RPAREN   { AttrList($1) }
	| READ LPAREN STRING_LIT RPAREN			{ Read($3) }
    | MAKESTRING LPAREN expr RPAREN    { MakeString($3) }
    

list_items:
    { Noitem }
    |  list_element                         { Item($1) }
    | list_element COMMA list_items        { Seq($1, Comma, $3) }  

list_element:
    NUM_LIT                      { LitIntElem($1) }
    | STRING_LIT                 { LitStrElem($1) }
    | LBRACK list_items RBRACK   { LitListOfList($2) }
    | LBRACE json_items RBRACE   { LitJsonOfList($2) }
	| BOOL_LIT                   { LitBoolElem($1) }
	| NULL						 { LitNullElem("null") }

json_items:
{ NoJsonItem }
| json_item                    { JsonItem($1)}
| json_item COMMA json_items   { JsonSeq($1, Comma, $3) }  

json_item:
    json_item_key COLON json_item_value { JsonValPair($1,Colon,$3) }

json_item_value:
    NUM_LIT                      { LitIntJsonVal($1) }
    | STRING_LIT                 { LitStrJsonVal($1) }
    | LBRACE json_items RBRACE   { LitJsonOfJson($2) }
    | LBRACK list_items RBRACK   { LitListOfJson($2) }
	| BOOL_LIT                   { LitBoolJsonVal($1) }
	| NULL						 { LitNullJsonVal("null") }

json_item_key:
 STRING_LIT                 { LitStrJsonKey($1) }

actuals_opt:
    /* nothing */   { [] }
    | actuals_list  { List.rev $1 }

actuals_list:
    expr                      { [$1] }
    | actuals_list COMMA expr { $3 :: $1 }