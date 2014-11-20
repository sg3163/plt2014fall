%{ open Ast %}

%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK
%token SEMI COMMA ASSIGN ACCESS
%token PLUS MINUS TIMES DIVIDE
%token EQ NEQ LT GT LEQ GEQ AND OR NOT 
%token COMMINUS COMPLUS MOD
%token RETURN IF THEN ELSE FOR END
%token IN READ TYPE TYPESTRUCT JOIN PRINT
%token MAKESTRING 
%token <int> LIT_INT
%token <string> LIT_STR
%token <string> LIT_JSON
%token <string> LIT_LIST
%token <string> ID
%token EOF

%nonassoc ASSIGN

%left OR
%left AND
%left EQ NEQ
%left LT GT LEQ GEQ
%left IN
%left COMMINUS COMPLUS
%left PLUS MINUS 
%left TIMES DIVIDE MOD
%right NOT
%left ACCESS
%left LBRACK RBRACK
%left LPAREN RPAREN

%start program
%type <Ast.program> program

%%

program:
    stmt_list { List.rev $1 }

stmt_list:
   /* nothing */  { [] }
 | stmt_list stmt { $2 :: $1 }

stmt:
    assignment SEMI { $1 }

assignment:
    ID ASSIGN expr     { Assign(Id($1), $3) }

expr:
    NUM_LIT            { NumLit($1) }
  | BOOLEAN_LIT        { BoolLit($1) }
  | CHAR_LIT           { CharLit($1) }
  | STRING_LIT         { ListCreate(List.map (fun x -> CharLit(x)) (explode $1)) }
  | ID                 { Id($1) }
  | expr PLUS   expr   { Binop($1, Add,     $3) }
  | expr MINUS  expr   { Binop($1, Sub,     $3) }
  | expr TIMES  expr   { Binop($1, Mult,    $3) }
  | expr DIVIDE expr   { Binop($1, Div,     $3) }
  | expr MOD    expr   { Binop($1, Mod,     $3) }
  | expr EQ     expr   { Binop($1, Equal,   $3) }
  | expr NEQ    expr   { Binop($1, Neq,     $3) }
  
