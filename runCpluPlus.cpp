#include "cPlusPlusCompiler.h"
int main() {
	// your code goes here
	cout << endl ; 
	cout << endl ; 
	string b  = "\"arpit\""; 
	CustType a ; 
	CustType* c = CustType :: parse(b , "STRING") ; 
	c -> print() ;
	cout << endl ; 
	cout << c -> getType (
		) ; 
	cout <<endl ; 
	CustType :: print (c) ;

	cout << endl ;  
	return 0;
}
	