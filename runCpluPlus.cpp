#include "cPlusPlusCompiler.h"
int main() {
	// your code goes here
	cout << endl ; 
	cout << endl ; 
	string b  = "\"arpit\""; 
	CustType a = CustType(b) ; 
	CustType* c = a.parse() ; 
	c -> print() ;
	cout << endl ; 
	return 0;
}
	