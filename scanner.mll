{ open Parser }

let letter = ['a' - 'z' 'A' - 'Z']
let digit = ['0' - '9']
let quote = '"'

rule token = parse
	[' ' '\r' '\n' '\t']	{ token lexbuf }	
	| "/*"			{ comment lexbuf }
	| '('			{ LPAREN }		
	| ')'			{ RPAREN }
	| '{'			{ LBRACE }
	| ',' 			{ COMMA }		
	| '}'			{ RBRACE }
	| '['			{ LBRACK }		
	| ']'			{ RBRACK }
	| '.'			{ ACCESS } 	
	| "++"			{ PLUS }		
	| "--"			{ MINUS }
	| "**"			{ TIMES }		
	| "//"			{ DIVIDE }
	| '='			{ ASSIGN }		
	| ';'			{ SEMI } 
	| "=="			{ EQ }			
	| "!="			{ NEQ }
	| '<'			{ LT }			
	| "<="			{ LEQ }
	| '>'			{ GT }			
	| ">="			{ GEQ }
	| "&&"			{ AND }			
	| "||"			{ OR }
	| '!'			{ NOT }			
	| '-' 			{ COMMINUS }
	| '+'     		{ COMPLUS } 
	| ':'			{ COLON }
	| '#'			{ HASH }
	| "%%" 			{ MOD }
  	| "func"		{ FUNC }			
  	| "if"			{ IF }		
	| "else"		{ ELSE }
	| "for"			{ FOR }			
	| "in"			{ IN }
	| "end"			{ END }			
	| "not in"		{ NOTIN }			
	| "read"    	{ READ }  
	| "write"		{ WRITE }
	| "print"    	{ PRINT }
	| "type"    	{ TYPE } 
	| "typeStruct"  { TYPESTRUCT }
	| "attrList"	{ ATTRLIST } 
	| "makeString" 	{ MAKESTRING }
	| "return"		{ RETURN }
	| "null" 		{ NULL }
	| eof			{ EOF }			(* do as microC *)
	| ('-'? digit+) | ('-'? digit* '.' digit+) as lit					{ NUM_LIT(float_of_string lit) }
	| quote [^'"']* quote as lit	{ STRING_LIT(lit) } 
	| "true" | "false" as lit { BOOL_LIT (lit) }
	| letter | (letter | digit | '_')* as id		{ ID(id) }
	| _ as char 		{ raise (Failure("illegal character " ^ Char.escaped char)) }

and comment = parse
	"*/"			{ token lexbuf }
	| _				{ comment lexbuf }