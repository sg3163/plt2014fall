include Ast

(*************************
**** SAST ****************
*************************)

(* types for inference *)
type t = 
    TVar of string
  | TNum
  | TChar
  | TBool

(* annotated expression *)
type aExpr =
    ANumLit of float * t
  | ABoolLit of bool * t
  | ACharLit of char * t
  | AId of string * bool * t
  | ABinop of aExpr * Ast.op * aExpr * t
  
(* annotated statment *)
and aStmt =
    AAssign of (aExpr * aExpr)
  
(* annotated program *)
type aProgram = aStmt list
