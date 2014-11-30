type op = Add | Sub | Mult | Div | Equal | Neq | Less | Leq | Greater | Geq | And | Or

type data_type =  StrType | IntType | BoolType | JsonType | ListType 

type sep = Comma

type colon = Colon

type list_expr = 
  ListItemInt of int
  | ListItemStr of string

type list_element = 
  LitIntElem of int
  | LitStrElem of string
	| LitList of items
	| LitJson of json_items
and items = 
    Item of list_element
  | Seq of list_element * sep * items
  | Noitem
and json_key_type =
	LitStrJsonKey of string
and json_item_value = 
  LitIntJsonVal of int
  | LitStrJsonVal of string
	| LitJson of json_items
	| LitList of items
and json_item =
	JsonValPair of json_key_type * colon * json_item_value
and json_items = 
    JsonItem of json_item
  | JsonSeq of json_item * sep * json_items
  | NoItem

type expr =
    LitInt of int
  | LitStr of string
  | LitJson of json_items
  | LitList of items
	| LitBool of string
	| MainRet of int
  | Id of string
  | Binop of expr * op * expr
  | Assign of string * expr
	| Call of string * expr list
	| ElemAccess of string * list_expr
	| TypeStruct of string
  | NoExpr
    
type var_decl = {
  vtype : data_type;
  vname : string;
  vexpr : expr;
}

type for_expr = 
    Forid of string

type loop_var = 
    LoopVar of string
type stmt =
    Block of stmt list
  | Expr of expr
  | Return of expr
  | Print of expr
  | If of expr * stmt * stmt
  | For of loop_var * for_expr * stmt  

type func_decl = {
    return : data_type;
    fname : string;
    formals : var_decl list;
    fnlocals : var_decl list;
    body : stmt list;
  }


type program = var_decl list * func_decl list