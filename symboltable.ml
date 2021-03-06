open Ast

module StringMap = Map.Make(String)

type env = {
    locals:         string StringMap.t;
	globals:        string StringMap.t;
	functions:      string list StringMap.t;
}

let string_of_vtype = function
	IntType -> "number"
  | StrType -> "string"
  | BoolType -> "bool"
  | JsonType -> "json"
  | ListType -> "list"
  | NoType -> "notype"

let find_variable name env =
	try StringMap.find name env.locals
	with Not_found -> try StringMap.find name env.globals
	with Not_found -> ""

let find_function name env =
	try StringMap.find name env.functions
	with Not_found -> []

let add_local name v_type env =
	if StringMap.mem name env.locals then StringMap.empty
	else StringMap.add name (string_of_vtype v_type) env.locals

let update_local name v_type env =
	if StringMap.mem name env.locals then StringMap.add name (string_of_vtype v_type) env.locals, "exist"
	else StringMap.add name (string_of_vtype v_type) env.locals, "added"

let add_global name v_type env =
	if StringMap.mem name env.globals then StringMap.empty
	else StringMap.add name (string_of_vtype v_type) env.globals

let update_global name v_type env =
	StringMap.add name (string_of_vtype v_type) env.globals

(* from the ast *)
let get_arg_type = function
	v -> string_of_vtype v.vtype

let add_function name return_type formals env =
	if StringMap.mem name env.functions then StringMap.empty
	else let f = List.map get_arg_type formals in
	StringMap.add name (string_of_vtype (return_type)::f) env.functions
