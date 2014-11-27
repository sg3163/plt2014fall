#include "cPlusPlusCompiler.h"
int main() {
	// your code goes here
	cout << endl ; 
	cout << endl ; 
	string b  = "arpit"; 
	CustType a ; 
	CustType* c = CustType :: parse( "arpitdfkjvdk" , "STRING") ; 
	c -> print() ;
	cout << endl ; 
	cout << c -> getType (
		) ; 
	cout <<endl ; 
	CustType :: print (CustType :: parse(b , "STRING")) ;
	cout << "\nfrom object\n"  ; 
	CustType :: print (c) ;
    cout << "\n printing 5 below \n" ;
    b = "5" ;
    
    c = CustType :: parse( b , "NUMBER") ;
    
    CustType :: print (c) ;
    cout << "\n printing 5 above \n" ;
    
	cout << endl ;  


	//Test the JSON
	cout << endl;
	cout << endl;

	string testString = "\
        { \
          \"name\" : \"name1\", \
          \"age\" : \"25\", \
          \"scores\" : \"[1, 2, 3]\", \
          \"sub_object\" : { \
            \"foo\" : \"abc\", \
            \"bar\" : \"123\" \
            } \
        } ";

	CustType *testJsonType = CustType :: parse(testString, "JSON");
	testJsonType -> print();
	cout << endl;
	cout << testJsonType -> getType();
	cout << endl;



	return 0;
}
