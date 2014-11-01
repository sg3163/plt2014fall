%{ open Ast %}

%token PLUS MINUS TIMES DIVIDE EOF
%token <int> LIT_INT
%left PLUS MINUS
%left TIMES DIVIDE
%start program
%type <Ast.program> program
%%
program:
program PLUS program { Binop($1, Add, $3) }
| program MINUS program { Binop($1, Sub, $3) }
| program TIMES program { Binop($1, Mult, $3) }
| program DIVIDE program { Binop($1, Div, $3) }
| LIT_INT { Lit($1) }