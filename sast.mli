type op_t = Add | Sub | Mult | Div | Equal | Neq | Less | Leq | Greater | Geq | And | Or 

type data_type_t = StrType | IntType | BoolType | JsonType | ListType

type sep_t = Comma

type colon_t = Colon

type list_expr_t = 
     ListItemInt of int
  | ListItemStr of string

type list_element_t = 
  LitIntElem of int
  | LitStrElem of string
	| LitList of items_t
	| LitJson of json_items_t
and items_t = 
    Item of list_element_t
  | Seq of list_element_t * sep_t * items_t
  | Noitem
and json_key_type_t =
	LitStrJsonKey of string
and json_item_value_t = 
  LitIntJsonVal of int
  | LitStrJsonVal of string
	| LitJson of json_items_t
	| LitList of items_t
and json_item_t =
	JsonValPair of json_key_type_t * colon_t * json_item_value_t
and json_items_t = 
    JsonItem of json_item_t
  | JsonSeq of json_item_t * sep_t * json_items_t
  | Noitem
	
type expr_t =
    LitInt of int
  | LitStr of string
	| LitJson of json_items_t
  | LitList of items_t
	| LitBool of string
  | Id of string
  | Binop of expr_t * op_t * expr_t
  | Assign of string * expr_t
  | Call of string * expr_t list
	| ElemAccess of string * list_expr_t
	| MainRet of int
  | NoExpr

type for_expr_t = 
    Forid of string

type loop_var_t = 
    Loopvar of string
<<<<<<< HEAD
  
=======

>>>>>>> f70b3854176b233639890816ce97b471d808082a
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