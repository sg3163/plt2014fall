{ open Parser }

let letter = ['a' - 'z' 'A' - 'Z']
let digit = ['0' - '9']
let quote = '"'

rule token = parse
	[' ' '\r' '\n' '\t']	{ token lexbuf }	| "/*"		{ comment lexbuf }
	| '('			{ LPAREN }		| ')'			{ RPAREN }
	| '{'			{ LBRACE }		| '}'			{ RBRACE }
	| "++"			{ PLUS }		| "--"			{ MINUS }
	| "**"			{ TIMES }		| "//"			{ DIVIDE }
	| '='			{ ASSIGN }		| ';'			{ SEMI }
	| "=="			{ EQ }			| "!="			{ NEQ }
	| '<'			{ LT }			| "<="			{ LEQ }
	| '>'			{ GT }			| ">="			{ GEQ }
	| '['			{ LBRACK }		| ']'			{ RBRACK }
	| "&&"			{ AND }			| "||"			{ OR }
	| '!'			{ NOT }			| '-' { COMMINUS }
	| '+'     { COMPLUS } | "%%" { MODULUS }
  | "if"			{ IF }			| '.' { DOT }
	| "else"		{ ELSE }
	| "then"		{ THEN }		
	| "for"			{ FOR }			| "in"			{ IN }
	| "end"			{ END }			| "not in"  {NOTIN}
	| "read"    { READ }  |  "print"    {PRINT}
	| "type"    { TYPE } |  "typeStruct" { TYPESTRUCT }
	| "join"    { JOIN } | "makeString" { MAKESTRING }
	| eof			{ EOF }			(* do as microC *)
	| digit+ as lit					{ LIT_INT(int_of_string lit) }
	| quote [^'"']* quote as lit	{ LIT_STR(lit) } 
	| "#{" _* "}#" as lit { LIT_JSON(lit) }
	| "[" _* "]" as lit { LIT_LIST(lit) }
	| letter | (letter | digit | '_')* as id		{ ID(id) }
	| _ as char 		{ raise (Failure("illegal character " ^ Char.escaped char)) }

and comment = parse
	"*/"			{ token lexbuf }
	| _				{ comment lexbuf}