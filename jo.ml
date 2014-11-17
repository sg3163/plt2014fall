open Sast
open Str

let rec string_of_expr = function
    LitInt(l) -> "convertToNumber(" ^ string_of_int l ^ ")"
(*  | LitStr(l) -> "convertToString(" ^ l ^ ")" *)
	| LitStr(l) -> l
	| LitJson(l) -> "convertToJson(" ^  l ^ ")"
	| LitList(l) -> "convertToList(" ^ l ^ ")"
  | Id(s) ->  s
  | Binop(e1, o, e2) ->
      string_of_expr e1 ^ " " ^
      ( match o with
          Add -> "+" | Sub -> "-" | Mult -> "*" | Div -> "/"
        | Equal -> "==" | Neq -> "!=") ^ " " ^ string_of_expr e2
  | Assign(v, e) -> "CustomType " ^ v ^ " = " ^ string_of_expr e
  | Noexpr -> ""

let string_of_vtype = function
 IntType -> "int"
  | StrType -> "string"
  | BoolType -> "int"

let string_of_vdecl vdecl = if vdecl.vexpr = Noexpr then
                              string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname ^ ";\n"
                            else
                              string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname ^ " = " ^ string_of_expr vdecl.vexpr ^ ";\n"


let rec string_of_stmt = function
    Expr(expr) -> if compare (string_of_expr expr) "" = 0 then "\n" else string_of_expr expr ^ ";\n"
  | Block(stmts) ->
      "{\n" ^ String.concat "" (List.map string_of_stmt stmts) ^ "\n}"
	| Print(expr) -> (*if expr_type = "string" then*)
                                "cout << " ^ string_of_expr expr ^ ";\n"
                         (*     else 
                                "cout << \"we will print json here\n\""		*)
			(* variable declrarations, has ;*)
(*let string_of_vdecl vdecl = if vdecl.vexpr = Noexpr then
                              string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname ^ ";\n"
                            else
                              string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname ^ " = " ^ string_of_expr vdecl.vexpr ^ ";\n"
*)
let string_of_program (vars) =
	"\n#include <iostream> \n using namespace std; \n int main() { \n"  ^
	
  String.concat "" (List.map string_of_vdecl vars) ^ "\n"
 ^	 "}" 

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