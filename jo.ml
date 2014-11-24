open Sast

let rec string_of_expr e = match e with
    LitInt(l) -> "CustType::parse(\"" ^ string_of_int l ^ "\",\"Number\")\n"
  | LitStr(l) -> "CustType::parse(" ^ l ^ ",\"String\")\n"
	| LitJson(l) -> "CustType::parse(\"" ^ Str.global_replace (Str.regexp "\"") "\\\"" l ^ "\",\"Json\")\n"
	| LitList(l) -> "CustType::parse(\"" ^ Str.global_replace (Str.regexp "\"") "\\\"" l ^ "\",\"List\")\n"
	| LitBool(l) -> "CustType::parse(\"" ^ l ^ "\",\"Bool\")\n"
  | Id(s) ->  s
  | Binop(e1, o, e2) ->
      string_of_expr e1 ^ " " ^
      ( match o with
          Add -> "+" | Sub -> "-" | Mult -> "*" | Div -> "/" | Or -> "||"
          | And -> "&&" | Geq -> ">=" | Leq -> "<=" | Greater -> ">" | Less -> "<"
        | Equal -> "==" | Neq -> "!=") ^ " " ^ string_of_expr e2
  | Assign(v, e) -> "CustomType " ^ v ^ " = " ^ string_of_expr e
  | NoExpr -> ""

let rec string_of_stmt = function
    Expr(expr) -> if compare (string_of_expr expr) "" = 0 then "\n" else string_of_expr expr
    | Return(expr) -> "return " ^ string_of_expr expr ^ ";"
		| Print(expr, expr_type) -> "print(" ^ string_of_expr expr ^ ");\n" 

let string_of_vtype = function
   IntType -> "CustType"
  | StrType -> "CustType"
  | BoolType ->"CustType"
  | ListType -> "CustType"
  | JsonType -> "CustType"

  
let string_of_vdecl vdecl = if vdecl.vexpr = NoExpr then
                              string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname ^ "\n"
                            else
                              string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname ^ " = " ^ string_of_expr vdecl.vexpr ^ ";\n"

let string_of_formaldecl vdecl = string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname

let string_of_fdecl fdecl =
  string_of_vtype fdecl.return ^ " " ^ fdecl.fname ^ "(" ^
    String.concat ", " (List.map string_of_formaldecl fdecl.formals) ^ ")\n{\n" ^
  String.concat "" (List.map string_of_vdecl fdecl.fnlocals) ^
  String.concat "" (List.map string_of_stmt fdecl.body) ^
  "}\n"
 
let string_of_program (vars, funcs) =
	"\n#include <iostream>\n#include \"cPlusPlusCompiler.h\"\nusing namespace std;\n\n " ^
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
