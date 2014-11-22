#include "cPlusPlusCompiler.h"
int main() {
	// your code goes here

	string b  = "\"arpit\""; 
	CustType a = CustType(b) ; 
	CustType c = a.parse() ; 
	
	return 0;
}
