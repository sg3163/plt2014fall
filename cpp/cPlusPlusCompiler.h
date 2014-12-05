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
string wstringToString (wstring w){
	string result = "" ;
	char x ; 
    for ( int i = 0  ; w[i] != '\0' ; i ++ ){
    	x = w[i] ; 
    	result += x ; 
    }
    return result ;
}
enum dataType { NUMBER , STRING , JSON, LIST , BOOL } ; 

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
	virtual CustType* getAttrList () {
		JSONObject::iterator it ;
		cout << "Accesing outside of Json Object "  ; 
		return NULL ;	
	}
	virtual CustType* getElement ( string key ) {
		cout << "In CustType, apparently not in JSON. Calling from some other type\n" ; 
		return  NULL ; 
	}
	virtual CustType* getElement ( int index ) {
		cout << "In CustType, apparently not in LIST. Calling from some other type\n" ; 
		return  NULL ; 
	}
	virtual void add (CustType* el) { 
		cout << "In CustType, apparently not in LIST. Calling from some other type\n" ; 
		
	}
	virtual void add (string  key, CustType* el) { 
		cout << "In CustType, apparently not in JSON. Calling from some other type\n" ; 

	}
};
class BoolType : public CustType { 
	public :
	
	bool da ; 
	int type ; 
	BoolType (bool da , int type ) : CustType (  ) { 
		
		this -> da = da ;
		this -> type = type ; 
	}

};
class NumType : public CustType { 
	
	
	public : 
	
	double da; 
	int type ; 
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
	
	public : 
	string da;
	int type ;  
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
	
	vector <CustType*> da; 
	int type ;
	JSONArray data ;
	public : 
	void convToListType() ;
	/*(ListType (vector <CustType> da , int type) : CustType (  ) { 
		
		this -> da = da ;
		this -> type = type ; 
	}*/
	ListType(){
		
	}
	ListType(JSONArray data, int type) : CustType()  { 
		this -> data = data ;
		this -> type = type ; 
		convToListType() ;
	}
	ListType(vector <string> v) : CustType () { 
		for (vector<string> :: iterator it = v.begin ( ); it != v.end () ; it ++ ){
			StringType* s = new StringType ( *it, STRING ) ; 
			this -> da.push_back (s) ; 	
		}
	}
	int getType () {
		return LIST ;
	}
	CustType* getElement (int index) {
		if ( index > da.size())
			return NULL ; 
		return da[index] ; 
	}
	void add (string  key, CustType* el) { 
		da.push_back (el) ;
	}
} ;


class JsonType : public CustType {

    //JSONObject da;
    int type;

    public :
    JSONObject data ;
    map <string , CustType* > da ;
    void convToJsonType () ;  
    JsonType(JSONObject data, int type) : CustType() {
        this -> data= data;
        this -> type = type;
        //this -> da = new map <string , CustType* >  ; 
        convToJsonType() ;
    }

    JsonType() {}

    void print() {
      JSONValue *value = new JSONValue(data);
      print_out(value->Stringify().c_str());
    }

    int getType() {
        return JSON;
    }
    CustType* getAttrList () ;
    JSONObject::iterator getBeginIterator (){
		JSONObject::iterator it  = data.begin();
		
		return it ;
	}
	JSONObject::iterator getEndIterator (){
		JSONObject::iterator it = data.end ();
		return it ;	
	}
	CustType * getElement (string key) {
		if ( da.find(key) == da.end())
			return NULL ;
		else
			return da[key] ;  
	}
	void add (string  key, CustType* el) { 
		da [key] = el ; 
	}

};
/******************************************************
		CLASS DEFINITIONS END HERE 
******************************************************/

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
void JsonType :: convToJsonType (){

	map <string , CustType* > a ; 
	
	for ( JSONObject::iterator iter  =  getBeginIterator() ; iter !=  getEndIterator () ; iter ++ ) {
		
		string key = wstringToString ( iter -> first ) ; 
		if ( (iter -> second)-> IsString () ) {
			string val = wstringToString ((iter-> second)-> AsString () ) ; 
			StringType* t = new StringType (val, STRING) ; 
			cout << val ;
			a[key] = t  ;  
		}
		else if ( (iter -> second)-> IsBool () ) {
			bool val = (iter-> second)-> AsBool ()  ; 
			BoolType* t = new BoolType (val, BOOL) ; 
			cout << val ;
			a[key] = t ;  
		}
		else if ( (iter -> second)-> IsNumber () ) {
			double val = (iter-> second)-> AsNumber ()  ; 
			NumType* t = new NumType (val, NUMBER) ; 
			cout << val ;
			a[key] = t ;  
		}
		else if ( (iter -> second)-> IsObject() ){
			JSONObject val = (iter-> second)-> AsObject () ; 
			JsonType* t = new JsonType ( val , JSON) ;
			a[key] =  t ; 
		}
		else if ( (iter) -> second -> IsArray () ){
			JSONArray val = (iter -> second )-> AsArray () ; 
			ListType* t = new ListType ( val , LIST ) ; 
			a[key] = t ; 
		}
		else { 
			cout << "Here I am, stuck in JsonType :: convToJsonType, I think the JSON library is screwing up, and throwing random types. " ; 
		}
		cout << endl ; 
		
	}

	da.insert ( a.begin() , a.end () ) ; 
	//return a ; 
}
void ListType :: convToListType () {

	for ( vector<JSONValue*>  :: iterator iter  = data.begin () ; iter !=  data.end () ; iter ++ ) {
		
		
		if ( (*iter) -> IsString () ) {
			string val = wstringToString ((*iter) -> AsString () ) ; 
			StringType* t = new StringType (val, STRING) ; 
			cout << val ;
			da.push_back(t) ;  
		}
		else if ( (*iter) -> IsBool () ) {
			bool val = (*iter) -> AsBool ()  ; 
			BoolType* t = new BoolType (val, BOOL) ; 
			cout << val ;
			da.push_back(t) ; 
		}
		else if ( (*iter) -> IsNumber () ) {
			double val = (*iter) -> AsNumber ()  ; 
			NumType* t = new NumType (val, NUMBER) ; 
			cout << val ;
			da.push_back(t) ; 
		}
		else if ( (*iter) -> IsObject() ){
			JSONObject val = (*iter) -> AsObject () ; 
			JsonType* t = new JsonType ( val , JSON) ;
			da.push_back(t) ; 
		}
		else if ( (*iter) -> IsArray () ){
			JSONArray val = (*iter) -> AsArray () ; 
			ListType* t = new ListType ( val , LIST ) ; 
			da.push_back(t) ; 
		}
		else { 
			cout << "Here I am, stuck in ListType :: convToListType, I think the JSON library is screwing up, and throwing random types. " ; 
		}
		cout << endl ; 
		
	}


}
CustType* JsonType :: getAttrList () { 

	vector <string> atrrListStr;
	for ( JSONObject::iterator iter  =  getBeginIterator() ; iter !=  getEndIterator () ; iter ++ ) {
		
		string keyString = wstringToString ( iter -> first ) ; 
		atrrListStr.push_back(keyString) ;
	}
	ListType* attrList = new ListType (atrrListStr) ; 
	return attrList ; 	
}
/*
int main() {
	// your code goes here

	string b  = "\"arpit\""; 
	CustType a = CustType(b) ; 
	CustType c = a.parse() ; 
	
	return 0;
}*/
