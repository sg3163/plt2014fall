open Sast

let rec string_of_expr e = match e with
    LitInt(l) -> "CustType::parse(\"" ^ string_of_int l ^ "\",\"NUMBER\")\n"
  | LitStr(l) -> "CustType::parse(" ^ l ^ ",\"STRING\")\n"
	| LitJson(l) -> "CustType::parse(\"" ^ Str.global_replace (Str.regexp "\"") "\\\"" l ^ "\",\"JSON\")\n"
	| LitList(l) -> "CustType::parse(\"" ^ Str.global_replace (Str.regexp "\"") "\\\"" l ^ "\",\"LIST\")\n"
	| LitBool(l) -> "CustType::parse(\"" ^ l ^ "\",\"BOOL\")\n"
	| MainRet(l) -> "0"
  | Id(s) ->  s
  | Binop(e1, o, e2) ->
      string_of_expr e1 ^ " " ^
      ( match o with
          Add -> "+" | Sub -> "-" | Mult -> "*" | Div -> "/" | Or -> "||"
          | And -> "&&" | Geq -> ">=" | Leq -> "<=" | Greater -> ">" | Less -> "<"
        | Equal -> "==" | Neq -> "!=") ^ " " ^ string_of_expr e2
  | Assign(v, e) -> v ^ " = " ^ string_of_expr e
	| Call(f, el) ->
      f ^ "(" ^ String.concat ", " (List.map string_of_expr el) ^ ");"
  | NoExpr -> ""

let rec string_of_stmt = function
    Expr(expr) -> if compare (string_of_expr expr) "" = 0 then "\n" else string_of_expr expr
    | Return(expr) -> (*if fname = "main" then "return 0 " else*) " return " ^ string_of_expr expr ^ ";"
		| Print(expr, expr_type) -> "CustType::print(" ^ string_of_expr expr ^ ");\n"
    | Block(stmts) ->
        "{\n" ^ String.concat "" (List.map string_of_stmt stmts) ^ "\n}"
    | If(e, s, Block([])) -> "if (" ^ string_of_expr e ^ ")\n" ^ string_of_stmt s
    | If(e, s1, s2) ->  "if (" ^ string_of_expr e ^ ")\n" ^
        string_of_stmt s1 ^ "else\n" ^ string_of_stmt s2

let string_of_vtype = function
   IntType -> "CustType*"
  | StrType -> "CustType*"
  | BoolType ->"CustType*"
  | ListType -> "CustType*"
  | JsonType -> "CustType*"

  
let string_of_vdecl vdecl = if vdecl.vexpr = NoExpr then
                              string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname ^ "\n"
                            else
                              string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname ^ " = " ^ string_of_expr vdecl.vexpr ^ ";\n"

let string_of_formaldecl vdecl = string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname

let string_of_fdecl fdecl = (if fdecl.fname = "main" then 
 "int " ^ fdecl.fname ^ "(" 
else
  string_of_vtype fdecl.return ^ " " ^ fdecl.fname ^ "(" )
 ^   String.concat ", " (List.map string_of_formaldecl fdecl.formals) ^ ")\n{\n" ^
  String.concat "" (List.map string_of_vdecl fdecl.fnlocals) ^
  String.concat "" (List.map string_of_stmt fdecl.body ) ^
	 "}\n"
 
let string_of_program (vars, funcs) =
	"\n#include <iostream>\n#include \"../cpp/cPlusPlusCompiler.h\"\nusing namespace std;\n\n" ^
	String.concat "\n" (List.map string_of_vdecl vars) ^ "\n" ^ 
  String.concat "\n" (List.map string_of_fdecl funcs) ^ "\n" 

let _ =
  (* first argument is the filename *)
  let fname = Sys.argv.(1) in
    (* check the extension *)
    let index = (if String.contains fname '.' then String.rindex fname '.' else 0 ) in
    let suffix = String.sub fname index 4 in
    if not (suffix = ".pjo") then raise (Failure ("Invalid type of source file."))
    else
      let input = open_in fname in
      let lexbuf = Lexing.from_channel input in
      let program = Parser.program Scanner.token lexbuf in
      (* added the type check *)
      let program_t = Typecheck.check_program program in
      let output = string_of_program program_t in
      print_endline output