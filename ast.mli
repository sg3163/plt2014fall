type op = Add | Sub | Mult | Div | Equal | Neq | Less | Leq | Greater | Geq | And | Or

type data_type =  StrType | IntType | BoolType | JsonType | ListType

type expr =
    LitInt of int
  | LitStr of string
  | LitJson of string
  | LitList of string
	| LitBool of string
	| MainRet of int
  | Id of string
  | Binop of expr * op * expr
  | Assign of string * expr
	| Call of string * expr list
  | NoExpr
  
type stmt =
    Block of stmt list
  | Expr of expr
  | Return of expr
	| Print of expr
  | If of expr * stmt * stmt
  
type var_decl = {
  vtype : data_type;
  vname : string;
  vexpr : expr;
}

type func_decl = {
    return : data_type;
    fname : string;
    formals : var_decl list;
    fnlocals : var_decl list;
    body : stmt list;
  }


type program = var_decl list * func_decl list