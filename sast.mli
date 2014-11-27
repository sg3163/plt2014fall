type op_t = Add | Sub | Mult | Div | Equal | Neq | Less | Leq | Greater | Geq | And | Or 

type data_type_t = StrType | IntType | BoolType | JsonType | ListType

type expr_t =
    LitInt of int
  | LitStr of string
	| LitJson of string
  | LitList of string
	| LitBool of string
  | Id of string
  | Binop of expr_t * op_t * expr_t
  | Assign of string * expr_t
  | Call of string * expr_t list
	| MainRet of int
  | NoExpr

type for_expr_t = 
    Forid of string

type loop_var_t = 
    Loopvar of string
  
type stmt_t =
	 Expr of expr_t
  | Block of stmt_t list
  | Return of expr_t
	| Print of expr_t * string
  | For of loop_var_t * for_expr_t * stmt_t
  | If of expr_t * stmt_t * stmt_t


type var_decl_t = {
  vtype : data_type_t;
  vname : string;
  vexpr : expr_t;
}

type func_decl_t = {
    return : data_type_t;
    fname : string;
    formals : var_decl_t list;
    fnlocals : var_decl_t list;
    body : stmt_t list;
  }

type program_t = var_decl_t list * func_decl_t list