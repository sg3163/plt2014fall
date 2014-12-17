type op = Add | Sub | Mult | Div | Equal | Neq | Less | Leq | Greater | Geq | And | Or | Concat | Minus | Mod | In | NotIn

type data_type =  StrType | IntType | BoolType | JsonType | ListType | NoType

type sep = Comma

type colon = Colon

type list_expr = 
  ListItemInt of float
  | ListItemStr of string

type list_element = 
  LitIntElem of float
  | LitStrElem of string
	| LitListOfList of items
	| LitJsonOfList of json_items
	| LitBoolElem of string
	| LitNullElem of string
and items = 
    Item of list_element
  | Seq of list_element * sep * items
  | Noitem
and json_key_type =
	LitStrJsonKey of string
and json_item_value = 
  LitIntJsonVal of float
  | LitStrJsonVal of string
	| LitJsonOfJson of json_items
	| LitListOfJson of items
	| LitBoolJsonVal of string
	| LitNullJsonVal of string
and json_item =
	JsonValPair of json_key_type * colon * json_item_value
and json_items = 
    JsonItem of json_item
  | JsonSeq of json_item * sep * json_items
  | NoJsonItem

type expr =
    LitInt of float
  | LitStr of string
  | LitJson of json_items
  | LitList of items
	| LitBool of string
	| LitNull of string
  | Id of string
  | Not of expr
  | Binop of expr * op * expr
	| Call of string * expr list
	| ElemAccess of string * expr
	| TypeStruct of string
	| AttrList of string
  | DataType of expr
	| Read of string
  | MakeString of expr
  | NoExpr
    
type var_decl = {
  vtype : data_type;
  vname : string;
  vexpr : expr;
}

type for_expr = 
    Forid of string
    | AttrList of string

type loop_var = 
    LoopVar of string

type stmt =
    Block of stmt list
  | Vdecl of var_decl
  | Expr of expr
  | Return of expr
  | Print of expr
  | If of expr * stmt * stmt
  | For of loop_var * for_expr * stmt  
	| Write of expr * string
  | ElemAssign of string * expr * expr

type func_decl = {
    return : data_type;
    fname : string;
    formals : var_decl list;
    body : stmt list;
  }

type program = var_decl list * func_decl list