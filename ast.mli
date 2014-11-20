type op = Add | Sub | Mult | Div |Mod |Equal | Neq 

type expr =
    NumLit of float
  | BoolLit of bool
  | CharLit of char
  | Id of string
  | Binop of expr * op * expr

and stmt =
    Assign of (expr * expr)

type program = stmt list
