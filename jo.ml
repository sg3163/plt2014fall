open Sast

let rec string_of_items = function
    Item(e) ->  string_of_elements e 

  | Seq(e, sep, i2) ->  string_of_elements e ^ ","
                    ^ (string_of_items i2)
  | Noitem -> ""
and string_of_elements = function
	LitIntElem(l) ->  string_of_int l 
  | LitStrElem(l) -> Str.global_replace (Str.regexp "\"") "\\\"" l 
	| LitListOfList(l) -> "[" ^ string_of_items l ^ "]"
	| LitJsonOfList(l) -> "{" ^ json_items l ^ "}"
	| LitBoolElem(l) ->  l 
  | LitNullElem(l) -> l 

and json_items = function
    JsonItem(e) ->  json_key_value e 
  | JsonSeq(e, sep, i2) ->  json_key_value e ^ ","
                    ^ (json_items i2)
  | NoJsonItem -> ""
and json_key_value = function
	JsonValPair(e1, colon, e2) ->  json_key e1 ^ ":" ^ json_value e2
and json_key = function 
	LitStrJsonKey(l) -> Str.global_replace (Str.regexp "\"") "\\\"" l 
and json_value = function
	LitIntJsonVal(l) -> string_of_int l
	| LitStrJsonVal(l) -> Str.global_replace (Str.regexp "\"") "\\\"" l 
	| LitJsonOfJson(l) -> "{" ^ json_items l ^ "}"
	| LitListOfJson(l) -> "[" ^ string_of_items l ^ "]"
	| LitBoolJsonVal(l) -> l
	| LitNullJsonVal(l) -> l 
let get_for_id e = match e
    with
    Forid(id) -> id

let string_of_loop_var_t = function
  LoopVar(l) -> l

let rec string_of_expr e = match e with
    LitInt(l) -> "CustType::parse(\"" ^ string_of_int l ^ "\",\"NUMBER\")"
  | LitStr(l) -> "CustType::parse(" ^ l ^ ",\"STRING\")"
  | LitJson(l) -> "CustType::parse(\"{" ^ json_items l ^ "}\",\"JSON\")"
  | LitList(l) -> "CustType::parse(\"[" ^ string_of_items l ^ "]\",\"LIST\")"
  | LitBool(l) -> "CustType::parse(\"" ^ l ^ "\",\"BOOL\")" 
  | LitNull(l) -> "CustType::parse(\"" ^ l ^ "\",\"NULL\")"
  | Id(s) ->  s
  | Not(e1) -> "!(*(" ^ string_of_expr e1 ^ "))"
  | Binop(e1, o, e2) ->
      
      ( match o with
          Add -> "CustType::add("^string_of_expr e1 ^ "," ^ string_of_expr e2 ^")"
          | Sub -> "CustType::subtract("^string_of_expr e1 ^ "," ^ string_of_expr e2 ^")"
          | Mult -> "CustType::multiply("^string_of_expr e1 ^ "," ^ string_of_expr e2 ^")"              
          | Div -> "CustType::divide("^string_of_expr e1 ^ "," ^ string_of_expr e2 ^")"
          | Mod -> "CustType::mod("^string_of_expr e1 ^ "," ^ string_of_expr e2 ^")"
          | Or -> "*("^string_of_expr e1 ^ ") " ^ "||" ^ " *(" ^ string_of_expr e2 ^")"              
          | And -> "*("^string_of_expr e1 ^ ") " ^ "&&" ^ " *(" ^ string_of_expr e2 ^")"  
          | Geq -> "*("^string_of_expr e1 ^ ") " ^ ">=" ^ " *(" ^ string_of_expr e2 ^")"                
          | Leq -> "*("^string_of_expr e1 ^ ") " ^ "<=" ^ " *(" ^ string_of_expr e2 ^")"   
          | Greater -> "*("^string_of_expr e1 ^ ") " ^ ">" ^ " *(" ^ string_of_expr e2 ^")"              
          | Less -> "*("^string_of_expr e1 ^ ") " ^ "<" ^ " *(" ^ string_of_expr e2 ^")"   
          | Equal -> "*("^string_of_expr e1 ^ ") " ^ "==" ^ " *(" ^ string_of_expr e2 ^")"      
          | Neq -> "*("^string_of_expr e1 ^ ") " ^ "!=" ^ " *(" ^ string_of_expr e2 ^")"   
          | Concat -> "CustType::concat(" ^ string_of_expr e1 ^ "," ^ string_of_expr e2 ^ ")"
          | Minus ->  string_of_expr e1 ^ "->minus(" ^ string_of_expr e2 ^ ")"
          | In -> string_of_expr e2 ^ "->contains("^ string_of_expr e1^")"
          | NotIn -> "!(*("^string_of_expr e2 ^ "->contains("^ string_of_expr e1^")))"
        ) 
  (*| Assign(v, e) -> v ^ " = " ^ string_of_expr e  ^ ";"*)
  | Call(f, el) ->
      f ^ "(" ^ String.concat ", " (List.map string_of_expr el) ^ ")"
	| ElemAccess(id, e) ->  id ^ "-> getElementByOcaml("^string_of_expr e ^ ")"           
	| TypeStruct(id) -> "CustType::typeStruct(" ^ id ^ ")"
	| AttrList(id) -> id ^ "-> getAttrList()"
  | DataType(expr, expr_type) -> "(" ^ string_of_expr expr ^ ")->getJoType()"
  | Read(str) -> "CustType::read(" ^ str ^ ")"
  | MakeString(expr, expr_type) -> "(" ^ string_of_expr expr ^ ")->makeString()"
  | NoExpr -> ""

let string_of_vtype = function
   IntType -> "CustType*"
  | StrType -> "CustType*"
  | BoolType ->"CustType*"
  | ListType -> "CustType*"
  | JsonType -> "CustType*"
  | NoType -> "CustType*"

  
let string_of_vdecl vdecl = 
    string_of_vtype vdecl.vtype ^ " " ^ vdecl.vname ^ " = " ^ string_of_expr vdecl.vexpr ^ ";\n"


let rec string_of_stmt = function
    Vdecl(vdecl) -> string_of_vdecl vdecl
    | Expr(expr) -> if compare (string_of_expr expr) "" = 0 then "\n" else string_of_expr expr ^ ";"

    | Return(expr) -> (*if fname = "main" then "return 0 " else*) " return " ^ string_of_expr expr ^ ";"

		| Print(expr, expr_type) -> "CustType::print(" ^ string_of_expr expr ^ ");\n"

		| Block(stmts) -> "{\n" ^ String.concat "" (List.map string_of_stmt stmts) ^ "\n}"

    | For(e1, e2, s1) -> "for (vector<CustType*> :: iterator  loopVar" ^ string_of_loop_var_t e1 ^ "  = " ^ get_for_id e2 ^ " -> getListBegin () ; loopVar" ^ string_of_loop_var_t e1 ^ " != " ^ get_for_id e2 ^ 
        " -> getListEnd () ;  " ^ "++loopVar" ^ string_of_loop_var_t e1 ^ ") {\n CustType* "^ string_of_loop_var_t e1 ^ " = *loopVar" ^ string_of_loop_var_t e1 ^ ";\n" 
      ^ string_of_stmt s1 ^ "\n}\n"

    | If(e, s, Block([])) -> "if ((" ^ string_of_expr e ^ ")->getBoolValue())\n" ^ string_of_stmt s

    | If(e, s1, s2) ->  "if ((" ^ string_of_expr e ^ ")->getBoolValue())\n" ^ string_of_stmt s1 ^ "else\n" ^ string_of_stmt s2

		| Write(expr, str) -> "CustType::write(" ^ string_of_expr expr ^ "," ^ str ^ ");\n"

    | Assign(v, e) -> v ^ " = " ^ string_of_expr e  ^ ";\n"

    | ElemAssign(id, expr1, expr2) -> id ^ "-> addByKey("^string_of_expr expr1 ^ ","^string_of_expr expr2 ^ ");\n"


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