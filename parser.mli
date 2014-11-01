type token =
  | PLUS
  | MINUS
  | TIMES
  | DIVIDE
  | EOF
  | LIT_INT of (int)

val expr :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Ast.expr
