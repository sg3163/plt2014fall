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

	CustType *testJsonType = CustType :: parse(testString, "LIST");
	cout << testJsonType -> toString() ; 
    
	int ctr = 0 ; 
	cout << "that was through new method \n" ; 
	for (vector<CustType*> :: iterator it = testJsonType -> getListBegin () ; it != testJsonType -> getListEnd () ; ++ it) {
		(*it) -> print ()  ; 
	}
	cout << "that was through new method \n" ; 
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
	cout << "testing concat below \n" ; 
	 (CustType::parse("[1,99]","LIST") ) -> print () ; 


	 cout << endl << endl << endl << endl << endl;
	 
	 cout << endl << "Testing getBoolValue()" << endl;
	 string boolean = "True";
	 CustType *b1 = CustType::parse(boolean, "BOOL");
	 bool boo1 = b1->getBoolValue();
	 if (boo1) { cout << "getBoolValue() worked" << endl; }
	 else { cout << "ERROR with getBoolValue()" << endl; }
	 

	 cout << endl << "Testing Operators" << endl;
	 
	 string ns1 = "0.5";
	 string ns2 = "0.25";
	 CustType *n1 = CustType::parse(ns1, "NUMBER");
	 CustType *n2 = CustType::parse(ns2, "NUMBER");

	 cout << "n1 Type:" << n1->getType() << endl;
	 cout << "n2 Type:" << n2->getType() << endl;
	 cout << "Testing NUMBER addition:" << endl;
	 cout << "n1 Value:"; CustType::print(n1);
	 cout << endl << "n2 Value:"; CustType::print(n2);
	 cout << endl;
	 *n1+=*n2;
	 cout << "n1+=n2 Type: " << n1->getType() << endl;
	 cout << "n1+=n2: "; CustType::print(n1);
	 cout << endl;

	 cout << "Testing NUMBER subtraction:" << endl;
	 cout << "n1 Value:"; CustType::print(n1);
	 cout << endl << "n2 Value:"; CustType::print(n2);
	 cout << endl;	 
	 *n1-=*n2;
	 cout << "n1-=n2:"; CustType::print(n1);
	 cout << endl;

	 cout << "Testing NUMBER multiplication:" << endl;
	 cout << "n1 Value:"; CustType::print(n1);
	 cout << endl << "n2 Value:"; CustType::print(n2);
	 cout << endl;
	 *n1*=*n2;
	 cout << "n1*=n2:"; CustType::print(n1);
	 cout << endl;

	 cout << "Testing NUMBER division:" << endl;
	 cout << "n1 Value:"; CustType::print(n1);
	 cout << endl << "n2 Value:"; CustType::print(n2);
	 cout << endl;
	 *n1/=*n2;
	 cout << "n1/=n2:"; CustType::print(n1);
	 cout << endl;

	 cout << endl << "Testing STRING concat:" << endl;
	 CustType *s1 = CustType::parse("Hello ", "STRING");
	 CustType *s2 = CustType::parse("World!", "STRING");
	 *s1+=*s2;
	 cout << "Should print Hello World!: "; CustType::print(s1);
	 cout <<endl;
	 
	 CustType *m1 = CustType::parse("1", "NUMBER");
	 CustType *m2 = CustType::parse("2", "NUMBER");
	 CustType *m3 = CustType::parse("3", "NUMBER");

	 CustType* m4 = CustType::add(CustType::add(m1, m2), m3);
	 cout << endl << "1 + 2 + 3 = ";
	 CustType::print(m4);

	 m4 = CustType::subtract(CustType::subtract(m3, m2), m1);
	 cout << endl << "3 - 2 - 1 = ";
	 CustType::print(m4);

	 cout << endl << "3 * 2 = ";
	 m4 = CustType::multiply(m3, m2);
	 CustType::print(m4);

	 cout << endl << "3 / 2 = ";
	 m4 = CustType::divide(m3, m2);
	 CustType::print(m4);

	 cout << endl << "3 % 2 = ";
	 m4 = CustType::mod(m3, m2);
	 CustType::print(m4);
	 
	 CustType *m5 = CustType::parse("Hello ", "STRING");
	 CustType *m6 = CustType::parse("World!", "STRING");
	 CustType *m7 = CustType::add(m5, m6);
	 cout << endl << "Hello + World!: ";
	 CustType::print(m7); cout << endl;	 

	 cout << endl;
	 if ((*m1==*m2)->getBoolValue()) { cout << "FAILED operator ==" << endl; }
	 else { cout << "Operator == Passed"  << endl; }
	 
	 if ((*m1!=*m2)->getBoolValue()) {cout << "Operator != Passed" << endl; }
	 else {cout << "FAILED operator !=" << endl; }

	 if ((*m1<*m2)->getBoolValue()) {cout << "Operator < Passed" << endl; }
	 else { cout << "FAILED operator <" <<endl; }

	 if ((*m1>*m2)->getBoolValue()) {cout << "FAILED operator >" << endl; }
	 else { cout << "Operator > Passed" << endl; }

	 if ((*m1<=*m1)->getBoolValue() && (*m1<=*m2)->getBoolValue()) {cout << "Operator <= Passed" << endl; }
	 else { cout << "FAILED Operator <=" << endl; }

	 if ((*m2>=*m2)->getBoolValue() && (*m2>=*m1)->getBoolValue()) {cout << "Operator >= Passed" << endl; }
	 else { cout << "FAILED Operator >=" << endl; }
	 
	 CustType *m8 = CustType::parse("True", "BOOL");
	 CustType *m9 = CustType::parse("False", "BOOL");
	 if((!*m8)->getBoolValue()) { cout << "FAILED Operator !" << endl; }
	 else { cout << "Operator ! Passed" << endl; }

	 if((*m8 && *m9)->getBoolValue()) { cout << "FAILED Operator &&" << endl; }
	 else { cout << "Operator && Passed" << endl; }

	 if ((*m8 || *m9)->getBoolValue()) {cout << "Operator || Passed" << endl; }
	 else { cout << "Failed Operator ||" << endl; }

	cout << testJsonType -> prettyPrint(0) ; 
    
	return 0;
}
