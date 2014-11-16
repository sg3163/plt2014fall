#include <iostream>
#include <string>
#include <vector> 
#include <map>

using namespace std;

enum dataType { NUMBER , STRING , JSON, LIST } ; 

class CustType {
	
	protected :
	int dt ; 
	string data ; 
	
	public :
	
	CustType (string data) {
	
		//this->dt = NUMBER ; 
		this->data = data ; 
	
		
	}
	CustType () { 
	
	}
	CustType parse () ; 
	
};

class NumType : public CustType { 
	
	double da; 
	public : 
	NumType (double da ) : CustType (  ) { 
		
		this -> da = da ;
		
	}
	
	
} ;
class StringType : public CustType { 
	
	string da; 
	public : 
	StringType (string da ) : CustType (  ) { 
		
		this -> da = da ;
		
	}
	StringType(){
		
	}
	
} ;
class ListType : public CustType { 
	
	vector <CustType> da; 
	public : 
	ListType (vector <CustType> da ) : CustType (  ) { 
		
		this -> da = da ;
		
	}
	ListType(){
		
	}
	
	
} ;
class JsonType : public CustType { 
	
	map <string, CustType> da; 
	public : 
	JsonType (map <string, CustType > da ) : CustType (  ) { 
		
		this -> da = da ;
		
	}
	JsonType () { 
		
	}
} ;

NumType getNum (string data){
	double num  = 0 ; 
	int decimal_bool = 0, decimal = 1 ; 
	for ( unsigned int i = 0 ; i < data.length() ; i ++ ){
		
		if ( ( data.at(i) < '0' || data.at(i) > '9' ) && data.at(i) != '.' )
			return NULL ; 
		else if (data.at(i) < '0' || data.at(i) > '9') {
			num *= 10 ; 
			num += (data.at(i) - '0' ) ; 
		}
		else 
			decimal_bool  = 1 ; 
		if (decimal_bool) 
			decimal *= 10 ; 
		
	}
	num = num / decimal ; 
	NumType t (num) ;
	return t ;
}
StringType getString (string data){
	StringType t; 
	if ( data.at(0) !='"' || data.at(data.length() - 1 ))  {

		return NULL ;

	}
	data.erase(0,1) ; 
	data.erase(data.length() - 1, 1) ;
	t = string (data)
	return t ;
}
ListType getList (string data){
	ListType t; 
	
	return t ; 
}
JsonType getJson (string data){
	JsonType t ; 
	return t ;
}
CustType CustType :: parse ()  { 
	
		if (data.at(0) == '"'){
			dt = STRING ; 
			return getString ( data ) ;
			
		}
		else if (data.at(0) == '['){
			dt = LIST ; 
			return getList ( data ) ;
		}
		else if (data.at(0) == '{'){
			dt = JSON ; 
			return getJson (data) ;
		}
		else{
			dt = NUMBER ; 
			return getNum (data) ;
		}
	}
int main() {
	// your code goes here

	string b ; 
	//CustType a = new CustType(b) ; 


	return 0;
}