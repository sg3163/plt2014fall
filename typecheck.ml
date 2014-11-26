open Ast
open Symboltable
(*adding testing comment *)
module StringMap = Map.Make(String)

let string_of_vtype = function
	 IntType -> "int"
  | StrType -> "string"
  | BoolType -> "bool"
  | JsonType -> "json"
  | ListType -> "list"

let get_sast_type = function
	Ast.JsonType -> Sast.JsonType
	| Ast.StrType -> Sast.StrType
	| Ast.IntType -> Sast.IntType
	| Ast.BoolType -> Sast.BoolType
	| Ast.ListType -> Sast.ListType


(* get variable type according to the name
 * raise error if no name matching in variable list *)
let get_vtype env id = 
	(* find_variable method is from the symbol table *)
	let t = find_variable id env in
	if t = "" then raise (Failure ("undefined variable: " ^ id)) else t




(* check the expression type can be used for
 * the corresponding argument according to definition
 * return the new expression list in expr_t for sast *)
let check_func_arg lst expr arg_t =
	if (snd expr) = arg_t then (fst expr)::lst else
	raise (Failure("function arguments do not match"))


(* it returns the expr and its type *)
let rec check_expr env = function
	Ast.LitInt(i) -> 
	
		Sast.LitInt(i), "int"

	| Ast.LitStr(s) -> 
		Sast.LitStr(s), "string"
		
	| Ast.LitJson(s) -> 
		Sast.LitJson(s), "json"
		
	| Ast.LitList(s) -> 
		Sast.LitList(s), "list"
		
	| Ast.LitBool(s) -> 
		Sast.LitBool(s), "bool"

	| Ast.Id(id) ->
		Sast.Id(id), (get_vtype env id)
	| Ast.MainRet(i) -> Sast.MainRet(i), "return"
	
	| Ast.Assign(id, e) ->
		let t = get_vtype env id in
		    
		Sast.Assign(id, (get_expr_with_type env e t)), "void"
	| Ast.Call(func, el) ->
		let args = find_function func env in	
		( match args with
			[] -> raise (Failure ("undefined function " ^ func))
			| hd::tl -> let new_list = try List.fold_left2 check_func_arg [] (List.map (check_expr env) el) tl
						   with Invalid_argument "arg" -> raise(Failure("unmatched argument list"))
				    in Sast.Call(func, List.rev new_list ), hd )
	| Ast.NoExpr -> Sast.NoExpr, "void"

and get_expr_with_type env expr t = 
	let e = check_expr env expr in
	fst e

	
let rec check_stmt env func = function
	Ast.Expr(expr) -> (Sast.Expr(fst (check_expr env expr))), env
	| Ast.Return(expr) -> let e = check_expr env expr in
			 (Sast.Return(fst e)), env 
	| Ast.Print(expr) -> let (expr, expr_type) = check_expr env expr in
							(Sast.Print(expr , expr_type)), env

and check_stmt_list env func = function 
	  [] -> []
	| hd::tl -> let s,e = (check_stmt env func hd) in s::(check_stmt_list e func tl)

(* convert a variable to its SAST type *)
let convert_to_sast_type x env = 
	let t = get_vtype env x.vname in
		let s_expr = 
		if not (x.vexpr = Ast.NoExpr) then
			get_expr_with_type env x.vexpr t
		else Sast.NoExpr
		in
	{
		Sast.vtype = get_sast_type x.vtype;
		Sast.vname = x.vname;
		Sast.vexpr = s_expr;
	}

let check_formal env formal = 
	let ret = add_local formal.vname formal.vtype env in
	if (string_of_vtype formal.vtype) = "void" then raise (Failure("cannot use void as variable type")) else
	if StringMap.is_empty ret then raise (Failure ("local variable " ^ formal.vname ^ " is already defined"))
	else let env = {locals = ret; globals = env.globals; functions = env.functions } in
	convert_to_sast_type formal env, env

let rec check_formals env formals = 
	match formals with 
	  [] -> []
	| hd::tl -> let f, e = (check_formal env hd) in (f, e)::(check_formals e tl) 


let check_local env local =
	let ret = add_local local.vname local.vtype env in
	if (string_of_vtype local.vtype) = "void" then raise (Failure("cannot use void as variable type")) else
	if StringMap.is_empty ret then raise (Failure ("local variable " ^ local.vname ^ " is already defined"))
	else let env = {locals = ret; globals = env.globals; functions = env.functions } in
	convert_to_sast_type local env, env

let rec check_locals env locals = 
	match locals with
	  [] -> []
	| hd::tl -> let l, e = (check_local env hd) in (l, e)::(check_locals e tl)

let check_function env func = 
	
	match List.hd (List.rev func.body) with
	Return(_) ->
	  	let env = {locals = StringMap.empty; globals = env.globals; functions = env.functions } in
		let ret = add_function func.fname func.return func.formals env in
		if StringMap.is_empty ret then raise (Failure ("function " ^ func.fname ^ " is already defined"))
		else let env = {locals = env.locals; globals = env.globals; functions = ret } in
		let f = check_formals env func.formals in
		let formals = List.map (fun formal -> fst formal) f in
		
		let l, env = 
		(match f with
			  [] -> let l = check_locals env func.fnlocals in
					 l, env
			| _ -> 	let env = snd (List.hd (List.rev f)) in
					let l = check_locals env func.fnlocals in
					l, env
		) in
		let fnlocals = List.map (fun fnlocal -> fst fnlocal) l in
		 (match l with
	            [] -> let body = check_stmt_list env func func.body in
	                { Sast.return = get_sast_type func.return; 
	                  Sast.fname = func.fname; 
	                  Sast.formals = formals; 
	                  Sast.fnlocals = fnlocals; 
	                  Sast.body = body
	                }, env

	            | _ -> let e = snd (List.hd (List.rev l)) in
	                   let body = check_stmt_list e func func.body in
	                  { Sast.return = get_sast_type func.return; 
	                    Sast.fname = func.fname; 
	                    Sast.formals = formals; 
	                    Sast.fnlocals = fnlocals; 
	                    Sast.body = body
	                  }, e 
          )

	| _ -> raise (Failure ("The last statement must be return statement"))


let rec check_functions env funcs = 
	match funcs with
	  [] -> []
	| hd::tl -> let f, e = (check_function env hd) in f::(check_functions e tl) 

let check_global env global =
	let ret = add_global global.vname global.vtype env in
	if StringMap.is_empty ret then raise (Failure ("global variable " ^ global.vname ^ " is already defined"))
	else let env = {locals = env.locals; globals = ret; functions = env.functions } in
	convert_to_sast_type global env, env

let rec check_globals env globals = 
	match globals with
	  [] -> []
	| hd::tl -> let g, e = (check_global env hd) in (g, e)::(check_globals e tl)

let check_program (globals, funcs) = 
 	let env = {	locals = StringMap.empty;
				globals = StringMap.empty;
				functions = StringMap.empty }
	in
	let g = check_globals env globals in
	(* make a list of globals *)
	let globals = List.map (fun global -> fst global) g in
	match g with
	(* no globals *)
	 [] -> (globals, (check_functions env (List.rev funcs)))
	(* get the envirnment from the last global *)
	| _ -> let e = snd (List.hd (List.rev g)) in (globals, (check_functions e (List.rev funcs)))