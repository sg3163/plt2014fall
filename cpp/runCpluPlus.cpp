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
          \"age\" : 25123, \
          \"BoolVal\" : True, \
          \"scores\" : [1, 2, 3 , { \
            \"foo\" : \"False\", \
            \"bar\" : \"123\" \
            } ], \
          \"sub_object\" : { \
            \"foo\" : \"True\", \
            \"bar\" : \"123\" \
            } \
        } ";
	string listString = "[1, 2, 3 , { \
            \"foo\" : \"False\", \
            \"bar\" : \"123\" \
            } ]" ;
	CustType *testJsonType = CustType :: parse(listString, "LIST");

    //string testString = "arpit" ;
	//CustType *testJsonType = CustType :: parse(testString, "JSON");
	//CustType *testJsonAcc = testJsonType -> getElement("sub_object")->getElement("foo");
	//testJsonType -> add ("class" , c) ; 
	//CustType *testJsonAcc = testJsonType -> getElement("scores");
	//cout << testJsonAcc -> getType () ; 
	//testJsonAcc -> print () ;
	int ctr = 0 ; 
	while ( CustType *t = testJsonType  -> getElement(ctr++) ) {
		//CustType *t = testJsonAcc -> getElement(ctr++) ; 
		//if ( ctr < 3 )
			t -> print () ;
			cout << " \n" ; 
	}
	//testJsonType -> print();
	cout << endl;
	//cout << testJsonType -> getType();
	cout << endl;
	/*
	for ( JSONObject::iterator iter  = testJsonType ->  getBeginIterator() ; iter != testJsonType -> getEndIterator () ; iter ++ ) {
		
		print_out ( (iter -> first).c_str() ) ; 
		print_out ( (iter-> second) -> Stringify ().c_str()) ; 
		cout   << endl ; 
	}*/


	return 0;
}
