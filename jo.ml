open Ast

let rec eval = function
  Lit(x) -> (string_of_int x)
| Binop(e1, op, e2) ->
  let v1 = eval e1 and v2 = eval e2 in
  match op with
  Add -> v1 ^ " + " ^ v2
  | Sub ->  v1 ^ " - " ^  v2
  | Mult ->  v1 ^ " * " ^  v2
  | Div ->  v1 ^ " / " ^ v2

let _ =
  (* first argument is the filename *)
  let fname = Sys.argv.(1) in
    (* check the extension *)
    let index = (if String.contains fname '.' then String.rindex fname '.' else 0 ) in
    let suffix = String.sub fname index 3 in
    if not (suffix = ".jo") then raise (Failure ("Invalid type of source file."))
    else
      let input = open_in fname in
      let lexbuf = Lexing.from_channel input in
      let program = Parser.program Scanner.token lexbuf in
      (* added the type check *)
      (*let program_t = Typecheck.check_program program in*)
      let output = eval program in
      print_endline output