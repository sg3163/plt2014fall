open Ast

let rec string_of_expr = function
    LitInt(l) -> string_of_int l
  | LitStr(l) -> l
  | Id(s) -> s
  | Binop(e1, o, e2) ->
      string_of_expr e1 ^ " " ^
      ( match o with
          Add -> "+" | Sub -> "-" | Mult -> "*" | Div -> "/"
        | Equal -> "==" | Neq -> "!=") ^ " " ^ string_of_expr e2
  | Assign(v, e) -> v ^ " = " ^ string_of_expr e
  | Noexpr -> ""

let rec string_of_stmt = function
    Expr(expr) -> if compare (string_of_expr expr) "" = 0 then "\n" else string_of_expr expr ^ ";\n"
  | Block(stmts) ->
      "{\n" ^ String.concat "" (List.map string_of_stmt stmts) ^ "\n}"

let string_of_program program =
  String.concat "" (List.map string_of_stmt program) ^ "\n"

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
      (*let program_t = Typecheck.check_program program in*)
      let output = string_of_program program in
      print_endline output