#include <iostream>
#include <string>
#include <vector> 
#include <map>
#include <algorithm>
#include "./JSON.h"

using namespace std;

//SimpleJSON uses 
void print_out(const wchar_t *output)
{
    wcout << output;
    wcout.flush();
}

enum dataType { NUMBER , STRING , JSON, LIST } ; 

class CustType {
	
	
	public :
	 
	static string data ; 
	
	CustType (string data) {
	
		//this->dt = NUMBER ; 
		this->data = data ; 
	
		
	}
	
	CustType () { 
	
	}
	static CustType* parse (string data, string type) ; 
	static string typeString ( CustType* t) ;
	static void print(CustType* data) ;
	virtual void print () {
		cout << "Printing in CustType, Ooops!\n Somebody needs to implement this in child class" ;
	}

	virtual int getType(){
		cout << "getting Type from CustType, Ooops!\n Somebody needs to implement this in child class" ;	
	}
	virtual JSONObject::iterator getBeginIterator (){
		JSONObject::iterator it ;
		cout << "Accesing outside of Json Object "  ; 
		return it ;
	}
	virtual JSONObject::iterator getEndIterator (){
		JSONObject::iterator it ;
		cout << "Accesing outside of Json Object "  ; 
		return it ;	
	}
	
};

class NumType : public CustType { 
	
	double da; 
	int type ; 
	public : 
	NumType (double da , int type) : CustType (  ) { 
		
		this -> da = da ;
		this -> type = type ; 
		
	}
	void print () {
		cout << da ; 
	}
	int getType () {
		return NUMBER ;
	}
} ;

class StringType : public CustType { 
	
	string da;
	int type ;  
	public : 
	StringType (string da , int type ) : CustType (  ) { 
		
		this -> da = da ;
		this -> type = type ; 
	}
	StringType(){
		
	}
	void print () {
		cout << da ; 
	}
	int getType () {
		return STRING ;
	}
} ;

class ListType : public CustType { 
	
	vector <CustType> da; 
	int type ; 
	public : 
	ListType (vector <CustType> da , int type) : CustType (  ) { 
		
		this -> da = da ;
		this -> type = type ; 
	}
	ListType(){
		
	}
	int getType () {
		return LIST ;
	}
	
} ;

/*
class JsonType : public CustType { 
	
	map <string, CustType> da; 
	int type ; 
	public : 
	JsonType (map <string, CustType > da , int type) : CustType (  ) { 
		
		this -> da = da ;
		this -> type = type ; 
	}
	JsonType () { 
		
	}
	int getType () {
		return JSON ;
	}
} ;
*/
class JsonType : public CustType {

    //JSONObject da;
    int type;

    public :
    JSONObject da ;
    JsonType(JSONObject da, int type) : CustType() {
        this -> da = da;
        this -> type = type;
    }

    JsonType() {}

    void print() {
      JSONValue *value = new JSONValue(da);
      print_out(value->Stringify().c_str());
    }

    int getType() {
        return JSON;
    }
    JSONObject::iterator getBeginIterator (){
		JSONObject::iterator it  = da.begin();
		
		return it ;
	}
	JSONObject::iterator getEndIterator (){
		JSONObject::iterator it = da.end ();
		return it ;	
	}
};

NumType* getNum (string data, int type ){
	double num  = 0 ; 
	int decimal_bool = 0, decimal = 1 ; 
	for ( unsigned int i = 0 ; i < data.length() ; i ++ ){
		if ( ( data.at(i) < '0' || data.at(i) > '9' ) && data.at(i) != '.' )
			return NULL ; 
		else if (data.at(i) > '0' || data.at(i) < '9') {
			num *= 10 ;
			num += (data.at(i) - '0' ) ; 
		}
		else 
			decimal_bool  = 1 ; 
		if (decimal_bool) 
			decimal *= 10 ; 
		
	}
	num = num / decimal ; 
	NumType* t  = new NumType(num , NUMBER) ;
	return t ;
}

StringType* getString (string data, int type){
	/*if ( data.at(0) !='"' || data.at(data.length() - 1 ) != '"')  {

		return NULL ;

	}*/
	//data.erase(0,1) ; 
	//data.erase(data.length() - 1, 1) ;
	
	StringType* t = new StringType (data, STRING) ; 

	return t ;
}

ListType* getList (string data, int type){
	ListType* t; 
	
	return t ; 
}

JsonType* getJson (string data, int type){

    std::string str = data;
    const char *cstr = str.c_str();

    JSONValue *value = JSON::Parse(cstr);
    JSONObject root;
    
    if (value == NULL) {
        return NULL;
    }
    else {
	if (value->IsObject() == false) {
	    return NULL;
	}
	else {
	    root = value->AsObject();
	}
    }

    JsonType* t = new JsonType(root, JSON);
    return t ;
}

CustType* CustType :: parse (string data, string type)  { 
	
		if (type == "STRING"){
			 
			return getString ( data , STRING) ;
	      	
		}
		else if (type == "LIST"){
			//CustType :: dt = LIST ; 
			return getList ( data , LIST ) ;
		}
		else if (type == "JSON"){
			//CustType :: dt = JSON ; 
			return getJson (data, JSON) ;
		}
		else{
			//CustType :: dt = NUMBER ; 
			return getNum (data,NUMBER) ;
		}
}

void CustType :: print ( CustType* data) { 
	data -> print () ;
}

string CustType :: typeString ( CustType* t) {

	int typeVal = t -> getType () ;
	string type = "" ;
	switch (typeVal) {
		case NUMBER : 	type = "NUMBER" ;
					  	break ;
		case STRING : 	type = "STRING" ;
					  	break ; 
		case JSON : 	type = "JSON" ;
						break ; 
		case LIST : 	type = "LIST" ;
						break ;	
	}
	return type ; 
}

/*
int main() {
	// your code goes here

	string b  = "\"arpit\""; 
	CustType a = CustType(b) ; 
	CustType c = a.parse() ; 
	
	return 0;
}*/
