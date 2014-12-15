type op_t = Add | Sub | Mult | Div | Equal | Neq | Less | Leq | Greater | Geq | And | Or | Concat | Minus | Mod

type data_type_t = StrType | IntType | BoolType | JsonType | ListType | NoType

type sep_t = Comma

type colon_t = Colon

type list_expr_t = 
     ListItemInt of int
  | ListItemStr of string

type list_element_t = 
  LitIntElem of int
  | LitStrElem of string
	| LitListOfList of items_t
	| LitJsonOfList of json_items_t
	| LitNullElem of string
	| LitBoolElem of string
and items_t = 
    Item of list_element_t
  | Seq of list_element_t * sep_t * items_t
  | Noitem
and json_key_type_t =
	LitStrJsonKey of string
and json_item_value_t = 
  LitIntJsonVal of int
  | LitStrJsonVal of string
	| LitJsonOfJson of json_items_t
	| LitListOfJson of items_t
	| LitBoolJsonVal of string
	| LitNullJsonVal of string
and json_item_t =
	JsonValPair of json_key_type_t * colon_t * json_item_value_t
and json_items_t = 
    JsonItem of json_item_t
  | JsonSeq of json_item_t * sep_t * json_items_t
  | NoJsonItem
	
type expr_t =
    LitInt of int
  | LitStr of string
	| LitJson of json_items_t
  | LitList of items_t
	| LitBool of string
	| LitNull of string 
  | Id of string
  | Not of expr_t
  | Binop of expr_t * op_t * expr_t
  | Assign of string * expr_t
  | Call of string * expr_t list
	| ElemAccess of string * list_expr_t
	| TypeStruct of string
	| AttrList of string
  | DataType of expr_t * string
	| Read of string
  | MakeString of expr_t * string
  | NoExpr

type for_expr_t = 
    Forid of string

type loop_var_t = 
    LoopVar of string

type stmt_t =
	 Expr of expr_t
  | Block of stmt_t list
  | Return of expr_t
	| Print of expr_t * string
  | For of loop_var_t * for_expr_t * stmt_t
  | If of expr_t * stmt_t * stmt_t
  | Ifin of string * string * stmt_t * stmt_t
	| Write of expr_t * string


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