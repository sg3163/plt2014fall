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
  | NoType	-> "notype"

let vtype_of_ocaml_type = function
  "int" -> IntType
  | "string" -> StrType
  | "bool" -> BoolType
  | "json" -> JsonType
  | "list" -> ListType
  | "notype" -> NoType
  | _ -> NoType

let get_sast_type = function
	Ast.JsonType -> Sast.JsonType
	| Ast.StrType -> Sast.StrType
	| Ast.IntType -> Sast.IntType
	| Ast.BoolType -> Sast.BoolType
	| Ast.ListType -> Sast.ListType
	| Ast.NoType -> Sast.NoType


(* get variable type according to the name
 * raise error if no name matching in variable list *)
let get_vtype env id = 
	(* find_variable method is from the symbol table *)
	let t = find_variable id env in
	if t = "" then raise (Failure ("undefined variable: " ^ id)) else t

(* get the type of expression:
 *  -> string if one of the two operands having string type
 *  -> int/boolean if both of the operands having the same type *)
let get_oper_type t1 t2 =
	if t1 = "json" then "json" else
	if t1 = "list" then "list" else
	if t1 = "string" || t2 = "string" then "string" else
	if t1 = "int" && t2 = "int" then "bool" else
	if t1 = "bool" && t2 = "bool" then "bool" else
	if t1 = "int" && t2 = "bool" then raise (Failure ("cannot use int with bool type inside expression")) else
	if t1 = "bool" && t2 = "int" then raise (Failure ("cannot use int with bool type inside expression")) else
	raise (Failure ("type error in get_oper_type"))

(* TODO ----- NEED TO have checks for boolean op and mathermatical op
	*)
	
let get_bool_equal_oper_type t1 t2 =
	if t1 = "notype" then "bool" else
	if t2 = "notype" then "bool" else
	if t1 = "json" && t2 = "json" then "bool" else
	if t1 = "list" && t2 = "list" then "bool" else
	if t1 = "string" && t2 = "string" then "bool" else
	if t1 = "int" && t2 = "int" then "bool" else
	if t1 = "bool" && t2 = "bool" then "bool" else
	raise (Failure ("Cannot Compare different types"))

let get_math_oper_type t1 t2 =
	if t1 = "notype" && t2 = "notype" then "int" else
	if t1 = "notype" then t2 else
	if t2 = "notype" then t1 else
	if t1 = "int" && t2 = "int" then "int" else
	raise (Failure ("Mathematical operator work on Number types only"))

let get_logical_oper_type t1 t2 =
	if t1 = "notype" then t2 else
	if t2 = "notype" then t1 else
	if t1 = "bool" && t2 = "bool" then "bool" else
	raise (Failure ("Logical operators can work on only bool types"))
	
let get_add_oper_type t1 t2 =
	if t1 = "notype" && t2 = "notype" then "int" else
	if t1 = "notype" then t2 else
	if t2 = "notype" then t1 else
	if t1 = "int" && t2 = "int" then "int" else
	if t1 = "string" && t2 = "string" then "string" else
	raise (Failure ("Add operator can work on integers or strings"))

let get_comp_oper_type t1 t2 =
	if t1 = "notype" then t2 else
	if t2 = "notype" then t1 else
	if t1 = "int" && t2 = "int" then "bool" else
	raise (Failure ("comparison operators can work on only Number types types"))

let check_listexpr env = function
 	Ast.ListItemInt(i) -> Sast.ListItemInt(i), "int"
	| Ast.ListItemStr(s) -> Sast.ListItemStr(s), "string"

(* Might have to do type checking *)
let check_func_arg lst expr arg_t = (fst expr)::lst 

let match_oper e1 op e2 =
	(* snd of expr is type *)
	(*let expr_t = get_oper_type (snd e1) (snd e2) in*)
	(match op with
	   Ast.Add -> let expr_t = get_add_oper_type (snd e1) (snd e2) in 
					if expr_t = "int" then (Sast.Binop(fst e1, Sast.Add, fst e2), "int") else
	   			if expr_t = "string" then (Sast.Binop(fst e1, Sast.Add, fst e2), "string") else
		  		raise (Failure ("Add operator can work on integers or strings"))
	 | Ast.Sub -> let expr_t = get_math_oper_type (snd e1) (snd e2) in 
			 if expr_t = "int" then (Sast.Binop(fst e1, Sast.Sub, fst e2), "int") else
		  raise (Failure ("Mathematical operator work on Number types only"))
	 | Ast.Mult -> let expr_t = get_math_oper_type (snd e1) (snd e2) in 
						 if expr_t = "int" then (Sast.Binop(fst e1, Sast.Mult, fst e2), "int") else
	 	   raise (Failure ("Mathematical operator work on Number types only"))
	 | Ast.Div -> let expr_t = get_math_oper_type (snd e1) (snd e2) in 
						 if expr_t = "int" then (Sast.Binop(fst e1, Sast.Div, fst e2), "int") else
		  raise (Failure ("Mathematical operator work on Number types only"))
	 | Ast.Mod -> let expr_t = get_math_oper_type (snd e1) (snd e2) in 
						 if expr_t = "int" then (Sast.Binop(fst e1, Sast.Mod, fst e2), "int") else
		  raise (Failure ("Mathematical operator work on Number types only"))
		  (* equal and not equal have special case for string comparison 
		  		we may need to add SAST and Eqs and Neqs *)
	 | Ast.Equal -> let expr_t = get_bool_equal_oper_type (snd e1) (snd e2) in 
								 if expr_t = "bool" then (Sast.Binop(fst e1, Sast.Equal, fst e2), "bool") else
                  raise (Failure ("Cannot Compare different types"))
	 | Ast.Neq -> let expr_t = get_bool_equal_oper_type (snd e1) (snd e2) in 
								 if expr_t = "bool" then (Sast.Binop(fst e1, Sast.Neq, fst e2), "bool") else
                  raise (Failure ("Cannot Compare different types"))
	 | Ast.Less -> let expr_t = get_comp_oper_type (snd e1) (snd e2) in 
									if expr_t = "bool" then (Sast.Binop(fst e1, Sast.Less, fst e2), "bool") else
                  raise (Failure ("Comparison operators can work on only Number types types")) 
	 | Ast.Leq -> let expr_t = get_comp_oper_type (snd e1) (snd e2) in 
									if expr_t = "bool" then (Sast.Binop(fst e1, Sast.Leq, fst e2), "bool") else
                  raise (Failure ("Comparison operators can work on only Number types types"))
	 | Ast.Greater -> let expr_t = get_comp_oper_type (snd e1) (snd e2) in 
									if expr_t = "bool" then (Sast.Binop(fst e1, Sast.Greater, fst e2), "bool") else
                  raise (Failure ("Comparison operators can work on only Number types types"))
	 | Ast.Geq -> let expr_t = get_comp_oper_type (snd e1) (snd e2) in 
								if expr_t = "bool" then (Sast.Binop(fst e1, Sast.Geq, fst e2), "bool") else
                  raise (Failure ("Comparison operators can work on only Number types types")) 
     | Ast.And -> let expr_t = get_logical_oper_type (snd e1) (snd e2) in 
								if expr_t = "bool" then (Sast.Binop(fst e1, Sast.And, fst e2), "bool") else
                  raise (Failure ("Logical operators can work on only bool types")) 
     | Ast.Or -> let expr_t = get_logical_oper_type (snd e1) (snd e2) in 
								if expr_t = "bool" then (Sast.Binop(fst e1, Sast.Or, fst e2), "bool") else
                  raise (Failure ("Logical operators can work on only bool types")) 
     | Ast.Concat -> (Sast.Binop(fst e1, Sast.Concat, fst e2), "list")
     | Ast.Minus -> if (snd e1) = "list" || (snd e1) = "notype" then (Sast.Binop(fst e1, Sast.Minus, fst e2), "list") else
					if (snd e1) = "json" || (snd e1) = "notype" then (Sast.Binop(fst e1, Sast.Minus, fst e2), "json") else
					raise (Failure ("Minus operator can work only on list or json"))
	)
	
let rec check_list_items env = function
	  Ast.Item(e) -> Sast.Item(fst (check_list_element env e))
	| Ast.Seq(e1, sep, e2) -> Sast.Seq(fst (check_list_element env e1), Sast.Comma, (check_list_items env e2))
	| Ast.Noitem -> Sast.Noitem
and check_list_element env = function 
	Ast.LitIntElem(i) -> 
	(*	let _ = print_string "in int" in *)
		Sast.LitIntElem(i), "int"
	| Ast.LitStrElem(s) -> 
	(*	let _ = print_string "in string " in*)
		Sast.LitStrElem(s), "string"
	| Ast.LitListOfList(items) -> 
	(*	let _ = print_string "in list " in*)
		Sast.LitListOfList(check_list_items env items), "list"
	| Ast.LitJsonOfList(items) -> 
	(*	let _ = print_string "in json " in*)
		Sast.LitJsonOfList(check_json_items env items), "json"
	| Ast.LitBoolElem(i) -> 
	(*	let _ = print_string "in int" in *)
		Sast.LitBoolElem(i), "bool"
	| Ast.LitNullElem(s) -> 
	(*	let _ = print_string "in string " in*)
		Sast.LitNullElem(s), "null"

and check_json_items env = function
	  Ast.JsonItem(e) -> Sast.JsonItem(check_json_keyValue env e)
	| Ast.JsonSeq(e1, sep, e2) -> Sast.JsonSeq((check_json_keyValue env e1), Sast.Comma, (check_json_items env e2))
	| Ast.NoJsonItem -> Sast.NoJsonItem
	
and check_json_keyValue env = function
	Ast.JsonValPair(e1, colon, e2) -> Sast.JsonValPair(fst (check_json_key env e1) , Sast.Colon, fst (check_json_value env e2))
	
and check_json_value env = function 
	Ast.LitIntJsonVal(i) -> 
	(*	let _ = print_string "in int" in *)
		Sast.LitIntJsonVal(i), "int"
	| Ast.LitStrJsonVal(s) -> 
	(*	let _ = print_string "in string " in*)
		Sast.LitStrJsonVal(s), "string"
	| Ast.LitJsonOfJson(items) -> 
	(*	let _ = print_string "in json " in*)
		Sast.LitJsonOfJson(check_json_items env items), "json"
	| Ast.LitListOfJson(items) -> 
	(*	let _ = print_string "in list " in*)
		Sast.LitListOfJson(check_list_items env items), "list"
	| Ast.LitBoolJsonVal(i) -> 
	(*	let _ = print_string "in int" in *)
		Sast.LitBoolJsonVal(i), "bool"
	| Ast.LitNullJsonVal(s) -> 
	(*	let _ = print_string "in string " in*)
		Sast.LitNullJsonVal(s), "null"
		
and check_json_key env = function 
	Ast.LitStrJsonKey(i) -> 
	(*	let _ = print_string "in int" in *)
		Sast.LitStrJsonKey(i), "string"

(* it returns the expr and its type *)
let rec check_expr env = function
	Ast.LitInt(i) -> 
	(*	let _ = print_string "in int" in *)
		Sast.LitInt(i), "int"

	| Ast.LitStr(s) -> 
	(*	let _ = print_string "in string " in*)
		Sast.LitStr(s), "string"
		
	| Ast.LitJson(items) -> 
	(*	let _ = print_string "in json " in*)
		Sast.LitJson(check_json_items env items), "json"
		
	| Ast.LitList(items) -> 
	(*	let _ = print_string "in list " in*)
		Sast.LitList(check_list_items env items), "list"
		
	| Ast.LitBool(s) -> 
	(*	let _ = print_string "in bool " in*)
		Sast.LitBool(s), "bool"
	
	| Ast.LitNull(s) -> 
		Sast.LitNull(s), "null" 

	| Ast.Id(id) ->
	(*	let _ = print_string "in iD" in*)
		Sast.Id(id), (get_vtype env id)
	
	| Ast.Not(e1) -> 
	(*	let _ = print_string "in Not " in*)
		let ret = check_expr env e1 in
		if (snd ret) = "bool" then Sast.Not(fst ret), "bool"
		else raise (Failure("! is applicable to bool expressions only"))
		
	| Ast.Binop(e1, op, e2) ->
	(*	let _ = print_string "in binop" in*)
		match_oper (check_expr env e1) op (check_expr env e2)

	(*| Ast.Assign(id, e) -> (*Assignment updates the type of the data type as well*)
	(*	let _ = print_string "in assign" in*)
		let ret = update_local id (get_expr_with_type env e) env in
		let env = {locals = ret; globals = env.globals; functions = env.functions } in
		Sast.Assign(id, fst (check_expr env e)), "void" *)
	| Ast.Call(func, el) ->
		(* find_function is from the symbol table *)
		let args = find_function func env in	(* return & arguments type list from definition *)
		( match args with
			[] -> raise (Failure ("undefined function " ^ func))
			| hd::tl -> let new_list = try List.fold_left2 check_func_arg [] (List.map (check_expr env) el) tl
						   with Invalid_argument "arg" -> raise(Failure("unmatched argument list"))
				    in Sast.Call(func, List.rev new_list ), hd )
	| Ast.ElemAccess(id, e) -> let t1 = get_vtype env id in
														let t2 = check_expr env e in
														if not ( (t1 = "notype" && (snd t2 = "string" || snd t2 = "notype" || snd t2 = "int")) || (t1 = "json" && (snd t2 = "string" || snd t2 = "notype")) || (t1="list" && (snd t2 ="int" || snd t2 = "notype")) ) 
															then raise (Failure("Elements of List and Json can be accessed via index and key respectively"))
														else
															Sast.ElemAccess (id, (fst t2)), "notype"
    | Ast.TypeStruct(id) ->	Sast.TypeStruct(id), "string"
	| Ast.AttrList(id) -> Sast.AttrList(id), "list"
	| Ast.DataType(expr) -> let (expr, expr_type) = check_expr env expr in
							(Sast.DataType(expr , expr_type)), "string"
	| Ast.Read(str) -> Sast.Read(str), "string"
	| Ast.MakeString(expr) -> let (expr, expr_type) = check_expr env expr in
							(Sast.MakeString(expr , expr_type)), "string"
	| Ast.NoExpr -> Sast.NoExpr, "notype"

and get_expr_with_type env expr = 
	let e = check_expr env expr in snd e

(* convert a variable to its SAST type *)
let convert_to_sast_type x env = 
	let s_expr = 
	if not (x.vexpr = Ast.NoExpr) then
		fst(check_expr env x.vexpr)
	else Sast.NoExpr
	in
	{
		Sast.vtype = get_sast_type x.vtype;
		Sast.vname = x.vname;
		Sast.vexpr = s_expr;
	}

let convert_to_vdecl_type x  = 
	{
		vtype = NoType;
		vname = x;
		vexpr = NoExpr;
	}
	
(*let find_vtype = function
	LitInt(d) -> IntType
  | LitStr(d) -> StrType
  | LitJson(d) -> JsonType
  | LitList(d) -> ListType
  | LitBool(d) -> BoolType
  | _ -> NoType *)

let check_formal env formal = 
	let ret = update_local formal.vname (vtype_of_ocaml_type (get_expr_with_type env formal.vexpr)) env in
	let env = {locals = fst ret; globals = env.globals; functions = env.functions } in
	convert_to_sast_type formal env, env

let rec check_formals env formals = 
	match formals with 
	  [] -> []
	| hd::tl -> let f, e = (check_formal env hd) in (f, e)::(check_formals e tl) 

let check_local env local =
	let ret = update_local local.vname (vtype_of_ocaml_type (get_expr_with_type env local.vexpr)) env in
	(*if (snd ret) = "" then Sast.Assign(local.vname, fst (check_expr env local.vexpr)), fst ret
	else *) let env = {locals = fst ret; globals = env.globals; functions = env.functions } in
	convert_to_sast_type local env, env

(*let check_local_for_loop_var env local =
	let ret = update_local local StrType env in
	let env = {locals = ret; globals = env.globals; functions = env.functions } in
	let loop_var_dec = convert_to_vdecl_type local in 
	convert_to_sast_type  loop_var_dec env, env*)

let rec check_locals env locals = 
	match locals with
	  [] -> []
	| hd::tl -> let l, e = (check_local env hd) in (l, e)::(check_locals e tl)

let check_forexpr env = function
	Ast.Forid(id) -> Sast.Forid(id), get_vtype env id

let check_loopvar env = function
	Ast.LoopVar(id) -> let loopId = convert_to_vdecl_type id in 
        let l, e = (check_local env loopId) in e, Sast.LoopVar(id)

let rec check_stmt env func = function
	Ast.Vdecl(vdecl) -> let ret = update_local vdecl.vname (vtype_of_ocaml_type (get_expr_with_type env vdecl.vexpr)) env in
							if (snd ret) = "exist" then let env = {locals = fst ret; globals = env.globals; functions = env.functions } in
							        Sast.Assign(vdecl.vname, fst (check_expr env vdecl.vexpr)), env 
								else let env = {locals = fst ret; globals = env.globals; functions = env.functions } in
									Sast.Vdecl(convert_to_sast_type vdecl env), env
	| Ast.Expr(expr) -> (Sast.Expr(fst (check_expr env expr))), env
	| Ast.Return(expr) -> let e = check_expr env expr in
			 (Sast.Return(fst e)), env 
	| Ast.Print(expr) -> let (expr, expr_type) = check_expr env expr in
							(Sast.Print(expr , expr_type)), env
	| Ast.Block(stmt_list) -> (Sast.Block(check_stmt_list env func stmt_list)), env
	| Ast.If(expr, stmt1, stmt2) ->	let e = check_expr env expr in
								if not(snd e = "bool") then raise (Failure ("The type of the condition in If statement must be boolean!"))
								(* if() {} else{} *) 
								else (Sast.If(fst e, fst (check_stmt env func stmt1), fst (check_stmt env func stmt2))), env
	| Ast.Ifin(var, id, stmt1, stmt2) ->
								if not(get_vtype env id = "list") then raise (Failure ("The If in statement must be looking in a list!"))
							else if (get_vtype env var = "list") then raise (Failure ("Checking a list in list is not supported"))
						else if (get_vtype env var = "json") then raise (Failure ("Checking a json in list is not supported"))
								else (Sast.Ifin(var, id, fst (check_stmt env func stmt1), fst (check_stmt env func stmt2))), env
	| Ast.For(expr1, expr2, stmt) -> let envNew, e1 = check_loopvar env expr1 in let e2 = check_forexpr envNew expr2 in
						   if not ( snd e2 = "list" ) then raise (Failure("The type of the expression in a For statement must be path"))
						   else (Sast.For( e1, fst e2, fst (check_stmt envNew func stmt))), envNew 
	| Ast.Write(expr, str) -> let (expr, expr_type) = check_expr env expr in
							(Sast.Write(expr , str)), env
	| Ast.ElemAssign(id, expr1, expr2) -> let t1 = get_vtype env id in
											let t2 = check_expr env expr1 in
														if not ( (t1 = "json" && snd t2 = "string") || (t1="list" && snd t2 ="int") )
															then raise (Failure("Elements of List and Json can be accessed via index and key respectively"))
														else
															Sast.ElemAssign (id, (fst t2), fst (check_expr env expr2)), env
	

and check_stmt_list env func = function 
	  [] -> []
	| hd::tl -> let s,e = (check_stmt env func hd) in s::(check_stmt_list e func tl)



(* this function will return the updated formals and body as per the abstract syntax tree, the return type, name and locals *)
let check_function env func = 
	match List.hd (List.rev func.body) with
	Return(_) ->
	  	let env = {locals = StringMap.empty; globals = env.globals; functions = env.functions } in
	  	(*  ret is new env *)
		let ret = add_function func.fname func.return func.formals env in
		if StringMap.is_empty ret then raise (Failure ("function " ^ func.fname ^ " is already defined"))
		(* update the env with functions from ret *)
		else let env = {locals = env.locals; globals = env.globals; functions = ret } in
		(* check the formal arguments, returns formal list appended with their env *)
		let f = check_formals env func.formals in
		(* get the list of formals from f *)
		let formals = List.map (fun formal -> fst formal) f in
		
		(* get the final env from the last formal *)
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
		 	(* empty f, no formal args *)
	            [] -> let body = check_stmt_list env func func.body in
	                { Sast.return = get_sast_type func.return; 
	                  Sast.fname = func.fname; 
	                  Sast.formals = formals; 
	                  Sast.fnlocals = fnlocals; 
	                  Sast.body = body
	                }, env

	            (* get the final env from the last formal *)
	            | _ -> let e = snd (List.hd (List.rev l)) in
	                   let body = check_stmt_list e func func.body in
	                  { Sast.return = get_sast_type func.return; 
	                    Sast.fname = func.fname; 
	                    Sast.formals = formals; 
	                    Sast.fnlocals = fnlocals; 
	                    Sast.body = body
	                  }, e 
          )

	| _ ->
		if not (func.fname = "main") then raise (Failure ("Return statement is missing for function '"^func.fname^"'"))
	else
	let env = {locals = StringMap.empty; globals = env.globals; functions = env.functions } in
	  	(*  ret is new env *)
		let ret = add_function func.fname func.return func.formals env in
		if StringMap.is_empty ret then raise (Failure ("function " ^ func.fname ^ " is already defined"))
		(* update the env with functions from ret *)
		else let env = {locals = env.locals; globals = env.globals; functions = ret } in
		(* check the formal arguments, returns formal list appended with their env *)
		let f = check_formals env func.formals in
		(* get the list of formals from f *)
		let formals = List.map (fun formal -> fst formal) f in
		
		(* get the final env from the last formal *)
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
		 	(* empty f, no fomal args *)
	            [] -> let body = check_stmt_list env func func.body in
	                { Sast.return = get_sast_type func.return; 
	                  Sast.fname = func.fname; 
	                  Sast.formals = formals; 
	                  Sast.fnlocals = fnlocals; 
	                  Sast.body = body
	                }, env

	            (* get the final env from the last formal *)
	            | _ -> let e = snd (List.hd (List.rev l)) in
	                   let body = check_stmt_list e func func.body in
	                  { Sast.return = get_sast_type func.return; 
	                    Sast.fname = func.fname; 
	                    Sast.formals = formals; 
	                    Sast.fnlocals = fnlocals; 
	                    Sast.body = body
	                  }, e 
          )

let check_main_function funcs =
	List.fold_left (fun s e -> s || (e.fname="main")) false funcs

let rec check_functions env funcs =
	match funcs with
	  [] -> []
	| hd::tl -> let f, e = (check_function env hd) in f::(check_functions e tl) 

(* returns the global and its env *)
let check_global env global =
 	(*let _ = print_string "in iD" in*)
	let ret = update_global global.vname (vtype_of_ocaml_type (get_expr_with_type env global.vexpr)) env in
	(* update the env with globals from ret *)
	let env = {locals = env.locals; globals = ret; functions = env.functions } in
	convert_to_sast_type global env, env

let rec check_globals env globals = 
	match globals with
	  [] -> []
	| hd::tl -> let g, e = (check_global env hd) in (g, e)::(check_globals e tl)

let check_program (globals, funcs) = 
	(* create the default environment *)
	if not (check_main_function funcs) then raise (Failure ("main function is not defined in the program"))
else
	let env = {	locals = StringMap.empty;
				globals = StringMap.empty;
				functions = StringMap.empty }
	in
	(* return the list of each global appended with its environments, the last global has the final env *)
	let g = check_globals env globals in 
	(* make a list of globals *)
	let globals = List.map (fun global -> fst global) g in
	match g with
	(* no globals *)
	 	[] -> (globals, (check_functions env (List.rev funcs)))
		(* get the envirnment from the last global *)
		| _ -> let e = snd (List.hd (List.rev g)) in (globals, (check_functions e (List.rev funcs)))


