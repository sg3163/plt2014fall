#include <iostream>
#include <string>
#include <vector> 
#include <map>
#include <algorithm>
#include "./JSON.h"
#include <sstream>

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

enum dataType { NUMBER , STRING , BOOL , JSON, LIST} ; 

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
	static void print (vector<CustType*>  :: iterator it ) ;
	virtual void print () {
		cout << "Printing in CustType, Ooops!\n Somebody needs to implement this in child class\n" ;
	}

	virtual int getType(){
		cout << "getting Type from CustType, Ooops!\n Somebody needs to implement this in child class" ;	
	}
	virtual map<string , CustType* >::iterator getBeginIterator (){
		map<string , CustType* >::iterator it ;
		cout << "Accesing outside of Json Object "  ; 
		return it ;
	}
	virtual map<string , CustType* >::iterator getEndIterator (){
		map<string , CustType* >::iterator it ;
		cout << "Accesing outside of Json Object "  ; 
		return it ;	
	}
	virtual vector <CustType*> :: iterator getListBegin () {
		vector <CustType*> :: iterator it ;
		cout << "Accesing outside of List Object "  ; 
		return it ;	
	}
	virtual vector <CustType*> :: iterator getListEnd () {
		vector <CustType*> :: iterator it ;
		cout << "Accesing outside of List Object "  ; 
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

	//Conv to String 
	virtual string toString () {
		return "\nInside CustType\n" ;
	}

	//conv to StrType
	virtual CustType* makeString () {
		return CustType :: parse ("\nInside CustType\n" , "STRING") ;
	}
	//print json in prettyPrint format
	virtual string prettyPrint(int offset){
		cout << "prettyPrint() is only valid on JSON type objects.\n" ; 
	}
	//append to a list
	virtual void add (CustType* el) { 
		cout << "In CustType, apparently not in LIST. Calling from some other type\n" ; 
		
	}

	//add item to map
	virtual void add (string  key, CustType* el) { 
		cout << "In CustType, apparently not in JSON. Calling from some other type\n" ; 

	}
	virtual CustType* contains (CustType* t) {
		cout << "Only defined for Lists. \n" ;
		return NULL ; 
	}
	virtual CustType* findIndex (CustType* t){
		cout << "Only defined for Lists. \n" ;
		return NULL ; 
	}
	virtual CustType* minus (CustType* t) {
		cout << "Only defined for Lists. \n" ; 
		return NULL ; 
	}
	//concat items and form a list
	static CustType* concat (CustType* t1, CustType* t2) ; 


	//Mathematical Operators
	//Add is valid for NUMBER + NUMBER and STRING + STRING (JO operator ++)
	//The remaining mathematical operators are valid only for NUMBER, NUMBER
	//Returns NULL pointer if arguments are of an invalid type. 
	static CustType* add(CustType* t1, CustType* t2);

	static CustType* subtract(CustType* t1, CustType* t2);

	static CustType* multiply(CustType* t1, CustType* t2);

	static CustType* divide(CustType* t1, CustType* t2);

	static CustType* mod(CustType* t1, CustType* t2);

 
	//Get c++ Boolean value from BoolType
	virtual bool getBoolValue() {
	  cout << "In CustType, only valid for BoolType\n";
	}

	//Operator Overloading:
	virtual CustType& operator+=(CustType& rhs)
	{
	  cout << "In CustType, only allowed in NUMBER and STRING\n";
	}

	virtual CustType& operator-=(CustType& rhs)
	{
	  cout << "In CustType, only allowed in NUMBER\n";
	}

	virtual CustType& operator*=(CustType& rhs)
	{
	  cout << "In CustType, only allowed in NUMBER\n";
	}

	virtual CustType& operator/=(CustType& rhs)
	{
	  cout << "In CustType, only allowed in NUMBER\n";
	}


	/*	
	virtual CustType operator!()
	{
	  cout << "In CustType, only allowed in BOOL\n";
	}
	*/


};

class BoolType : public CustType { 
	public :
	
	bool da ; 
	int type ; 
	BoolType (bool da , int type ) : CustType (  ) { 
		
		this -> da = da ;
		this -> type = type ; 
	}
	int getType () {
		return BOOL ;
	}

	bool getBoolValue()
	{
	  return da;
	}

	void print () {
	  if(da) { cout << "True" ; }
	  else { cout << "False" ; }
	}
	string toString () { 
		string ret = "False"; 
		if ( da)
			ret = "True" ;
		return ret ; 
	}
	CustType* makeString () {
		string ret = "False"; 
		if ( da)
			ret = "True" ;
		return CustType :: parse (this -> toString() , "STRING") ; 
	}

	/*
	CustType operator!()
	{
	  CustType *t1 = this;
	  BoolType* temp = dynamic_cast<BoolType*>(t1);
	  temp->da = !(temp->da);
	  return temp;
	}
	*/
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
	string toString () { 
		string ret ; 
		ostringstream strs;
		strs << this -> da;
		ret = strs.str() ;
		return ret ; 
	}
	CustType* makeString () {
		
		return CustType :: parse( this -> toString() , "STRING" ) ; 
		
	}
	NumType& operator+=(CustType& rhs)
        {
	  CustType& t1 = rhs;
	  NumType& temp = dynamic_cast<NumType&>(t1);
	  da+=temp.da;
	  return *this;
	}

	NumType& operator-=(CustType& rhs)
        {
	  CustType& t1 = rhs;
	  NumType& temp = dynamic_cast<NumType&>(t1);
	  da-=temp.da;
	  return *this;
	}

	NumType& operator*=(CustType& rhs)
        {
	  CustType& t1 = rhs;
	  NumType& temp = dynamic_cast<NumType&>(t1);
	  da*=temp.da;
	  return *this;
	}

	NumType& operator/=(CustType& rhs)
        {
	  CustType& t1 = rhs;
	  NumType& temp = dynamic_cast<NumType&>(t1);
	  da/=temp.da;
	  return *this;
	}

	bool operator==(CustType *rhs)
	{
	  NumType *temp1 = dynamic_cast<NumType *>(this);
	  NumType *temp2 = dynamic_cast<NumType *>(rhs);

	  return ((temp1->da)==(temp2->da));
	}

	bool operator<(CustType *rhs)
	{
	  NumType *temp1 = dynamic_cast<NumType *>(this);
	  NumType *temp2 = dynamic_cast<NumType *>(rhs);

	  return ((temp1->da)<(temp2->da));
	}

	bool operator>(CustType *rhs)
	{
	  NumType *temp1 = dynamic_cast<NumType *>(this);
	  NumType *temp2 = dynamic_cast<NumType *>(rhs);

	  return ((temp1->da)>(temp2->da));
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
	string toString () {
		return da ;
	}
	CustType* makeString () {
		return CustType :: parse(this -> toString() , "STRING" ) ; 
		
	}
	void print () {
		cout << da ; 
	}
	int getType () {
		return STRING ;
	}

	StringType& operator+=(CustType& rhs)
        {
	  CustType& t1 = rhs;
	  StringType& temp = dynamic_cast<StringType&>(t1);
	  da+=temp.da;
	  return *this;
	}
	
} ;

class ListType : public CustType { 
	
	
	int type ;
	JSONArray data ;
	public : 
	vector <CustType*> da; 
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
	void print () {
		for (vector<CustType*> :: iterator it = da.begin () ; it != da.end () ; ++ it) {
			(*it) -> print () ; 
		} 
	}
	string toString () { 
		string ret  = "[ " ; 
		for (vector<CustType*> :: iterator it = da.begin () ; it != da.end () ; ++ it) {
			if ( it != da.begin () )
				ret += " ," ; 
			ret += (*it) -> toString () ;

		}
		ret += " ]" ; 
		return ret ; 

	}
	CustType* makeString () {
		
		return CustType :: parse(this -> toString() , "STRING" ) ; 
		
	}
	CustType* getElement (int index) {
		
		if ( index >= da.size())
			return NULL ; 
		return da[index] ; 
	}
	vector<CustType*> :: iterator getListBegin(){
		return da.begin() ; 
	}
	vector<CustType*> :: iterator getListEnd (){
		return da.end() ; 
	}
	void add ( CustType* el ) { 
		da.push_back (el) ;
	}
	bool operator==(CustType *rhs)
	{
	  NumType *temp1 = dynamic_cast<NumType *>(this);
	  NumType *temp2 = dynamic_cast<NumType *>(rhs);

	  return ((temp1->da)==(temp2->da));
	}
	//returns index of element if found, otherwise returns -1 
	CustType* findIndex (CustType* c) ; 
	CustType* minus (CustType* c) ; 
	CustType* contains (CustType * c) ;

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
    string toString () {
    	string ret = "{ " ; 
    	for ( map<string , CustType* > :: iterator it = (this -> da).begin() ; it != (this -> da).end() ; ++ it ) {
    		if ( it != (this -> da).begin() )
    			ret += " , " ; 
    		ret += "\"" ;
    		ret += it -> first ; 
    		ret += "\"" ;

    		ret += " : " ;
    		string valStr =  (it -> second ) -> toString ();
    		if ( (it -> second) -> getType () == STRING) {
    			ret += "\"" ; 
    			ret += valStr ;
    			ret += "\"" ; 
    		}
    		else {
    			ret += valStr ;
    		}
    	}
    	ret += " }" ;
    	return ret ; 

    }
    CustType* makeString () { 
    	return CustType :: parse ( this -> toString () , "STRING") ; 
    }

    string prettyPrint (int offset) {
    	string ret = "" ; 
    	string offsetTabs = "" ; 
    	for ( int i = 1 ; i <= offset ; i ++) {
    		offsetTabs += "\t" ; 
    	}

    	ret += offsetTabs ;
    	ret += "{\n" ;
    	string insideJsonPrint = prettyPrintJsonUtility (this , offset + 1) ;
    	ret += insideJsonPrint ; 
    	ret += offsetTabs ; 
    	ret += "}\n" ;
    	return ret ; 
    }
    string prettyPrintJsonUtility ( JsonType* t , int offset) {
    	string ret = ""  ;
    	string offsetTabs = "" ; 
    	for ( int i = 1 ; i <= offset ; i ++) {
    		offsetTabs += "\t" ; 
    	} 
	    for ( map<string , CustType* > :: iterator it = (t -> da).begin() ; it != (t -> da).end() ; ++ it ) {
	    	string el = "" ;
	    	if ( it != (t -> da).begin() )
    			el += ",\n" ; 
    		el += offsetTabs ; 
	    	el += "\"" ;
    		el += it -> first ; 
    		el += "\"" ;

    		el += ": " ;
    		if ( (it -> second )  -> getType() < JSON){
				el +=  (it -> second ) -> toString ();
			}
			else if ((it -> second ) -> getType () == JSON){
				el += "{\n" ; 
				el += prettyPrintJsonUtility ( (JsonType*)(it -> second ), offset + 1 ) ; 
				el += offsetTabs ; 
				el += "}" ; 
			}
			else{
				el += "[\n" ; 
				el += prettyPrintListUtility ( (ListType*)(it -> second) , offset + 1 ) ; 
				el += offsetTabs ; 
				el += "]" ; 
			}
			ret += el ;	
		}
	    ret += "\n" ; 
	    return ret ; 
	}
    string prettyPrintListUtility( ListType* t, int offset){
    	string ret = ""  ;
    	string offsetTabs = "" ; 
    	for ( int i = 1 ; i <= offset ; i ++) {
    		offsetTabs += "\t" ; 
    	} 
	    for (vector<CustType*> :: iterator it = (t -> da).begin () ; it != (t -> da).end () ; ++ it ) {
	    	string el = "" ;
	    	if ( it != (t-> da).begin() )
    			el += ",\n" ; 
    		el += offsetTabs ; 

    		if ( (*it) -> getType() < JSON){
				el += (*it) -> toString ();
			}
			else if ((*it) -> getType () == JSON){
				el += "{\n" ; 
				el += prettyPrintJsonUtility ((JsonType*)(*it), offset + 1 ) ; 
				el += offsetTabs ; 
				el += "}" ; 
			}
			else{
				el += "[\n" ; 
				el += prettyPrintListUtility ((ListType*)(*it), offset + 1 ) ; 
				el += offsetTabs ; 
				el += "]" ;
			}
			ret += el ;	
		}
	    ret += "\n" ; 
	    return ret ; 
    }
    int getType() {
        return JSON;
    }
    CustType* getAttrList () ;

    vector<string> getStringAttrList () {
    	vector <string> atrrListStr;
		for ( map<string , CustType* > ::iterator iter  =  getBeginIterator() ; iter !=  getEndIterator () ; iter ++ ) {
		
			string keyString =  iter -> first  ; 
			atrrListStr.push_back(keyString) ;
		}
		return atrrListStr ;
    }
    
    map<string , CustType* >::iterator getBeginIterator (){
		map<string , CustType* > :: iterator it  = (this -> da).begin();
		
		return it ;
	}
	
	map<string , CustType* >::iterator getEndIterator (){
		map<string , CustType* > :: iterator it = (this -> da ).end ();
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


//Commented method does not handle fractions. Alternate method is included below.
/*
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
*/

NumType* getNum(string data, int type)
{
  const char *cstr = data.c_str();
  char* pEnd;
  double num = strtod(cstr, &pEnd);
  NumType* t = new NumType(num, NUMBER);
  return t;
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

	return (ListType *) t -> getElement ("List"); 
}

BoolType* getBool (string data , int type) {
	BoolType * t = new BoolType ( data == "True" ? 1 : 0 , BOOL ) ; 
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

			data = "{ \"List\" : " + data + " }" ;
			//CustType :: dt = LIST ; 
			return getList ( data , LIST ) ;
		}
		else if (type == "JSON"){
			//CustType :: dt = JSON ; 
			return getJson (data, JSON) ;
		}
		else if (type == "BOOL"){
			return getBool (data, JSON) ;
		}
		else{
			//CustType :: dt = NUMBER ; 
			return getNum (data, NUMBER) ;
		}
}

void CustType :: print ( CustType* data) { 
	data -> print () ;
}

void CustType :: print (vector<CustType*>  :: iterator it ) {
	(*it) -> print () ; 
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
	
	for ( JSONObject::iterator iter  = (this -> data).begin() ; iter !=  (this -> data).end () ; iter ++ ) {
		
		string key = wstringToString ( iter -> first ) ; 
		if ( (iter -> second)-> IsString () ) {
			string val = wstringToString ((iter-> second)-> AsString () ) ; 
			StringType* t = new StringType (val, STRING) ; 
			//cout << val ;
			a[key] = t  ;  
		}
		else if ( (iter -> second)-> IsBool () ) {
			bool val = (iter-> second)-> AsBool ()  ; 
			BoolType* t = new BoolType (val, BOOL) ; 
			//cout << val ;
			a[key] = t ;  
		}
		else if ( (iter -> second)-> IsNumber () ) {
			double val = (iter-> second)-> AsNumber ()  ; 
			NumType* t = new NumType (val, NUMBER) ; 
			//cout << val ;
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
		//cout << endl ; 
		
	}

	da.insert ( a.begin() , a.end () ) ; 
	//return a ; 
}

void ListType :: convToListType () {

	for ( vector<JSONValue*>  :: iterator iter  = data.begin () ; iter !=  data.end () ; iter ++ ) {
		
		
		if ( (*iter) -> IsString () ) {
			string val = wstringToString ((*iter) -> AsString () ) ; 
			StringType* t = new StringType (val, STRING) ; 
			//cout << val ;
			da.push_back(t) ;  
		}
		else if ( (*iter) -> IsBool () ) {
			bool val = (*iter) -> AsBool ()  ; 
			BoolType* t = new BoolType (val, BOOL) ; 
			//cout << val ;
			da.push_back(t) ; 
		}
		else if ( (*iter) -> IsNumber () ) {
			double val = (*iter) -> AsNumber ()  ; 
			NumType* t = new NumType (val, NUMBER) ; 
			//cout << val ;
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
		//cout << endl ; 
		
	}


}

CustType* JsonType :: getAttrList () { 

	vector <string> atrrListStr;
	for ( map<string , CustType* > ::iterator iter  =  getBeginIterator() ; iter !=  getEndIterator () ; iter ++ ) {
		
		string keyString =  iter -> first  ; 
		atrrListStr.push_back(keyString) ;
	}
	ListType* attrList = new ListType (atrrListStr) ; 
	return attrList ; 	
}

CustType* CustType :: concat (CustType* t1, CustType* t2){
		if ( t1 -> getType () == LIST) {
			t1 -> add ( t2 ) ; 
			return t1;
		}
		CustType* t = new ListType ;
		t -> add (t1) ;
		t -> add (t2) ; 
		return t ; 
	}

// Mathematical Operators for NumType

CustType* CustType::add(CustType* t1, CustType* t2) {
  if( (t1->getType() == NUMBER) && (t2->getType() == NUMBER) ) {
    
    NumType *temp1 = dynamic_cast<NumType*>(t1);
    NumType *temp2 = dynamic_cast<NumType*>(t2);
    
    double num = (temp1->da) + (temp2->da);
    NumType *t = new NumType(num, NUMBER);
    return t;  
  }
  else if( (t1->getType() == STRING) && (t2->getType() == STRING) ) {
    StringType *temp1 = dynamic_cast<StringType*>(t1);
    StringType *temp2 = dynamic_cast<StringType*>(t2);

    string str = (temp1->da) + (temp2->da);
    StringType *t = new StringType(str, STRING);
    return t;
  }
  else {
    cout << "ERROR: Add only allowed for NUMBER, NUMBER or STRING, STRING" << endl;
    return NULL;
  }

}

CustType* CustType::subtract(CustType* t1, CustType* t2) {
	if( (t1->getType() == NUMBER) && (t2->getType() == NUMBER) ) {
		NumType *temp1 = dynamic_cast<NumType*>(t1);
		NumType *temp2 = dynamic_cast<NumType*>(t2);

		double num = (temp1->da) - (temp2->da);
		NumType *t = new NumType(num, NUMBER);
		return t;  
	}
	else {
		cout << "ERROR: Subtract only allowed for NUMBER, NUMBER" << endl;
		return NULL;
	}
}

CustType* CustType::multiply(CustType* t1, CustType* t2) {
  if( (t1->getType() == NUMBER) && (t2->getType() == NUMBER) ) {
    NumType *temp1 = dynamic_cast<NumType*>(t1);
    NumType *temp2 = dynamic_cast<NumType*>(t2);
    
    double num = (temp1->da) * (temp2->da);
    NumType *t = new NumType(num, NUMBER);
    return t;  
  }
  else {
    cout << "ERROR: Multiply only allowed for NUMBER, NUMBER" << endl;
    return NULL;
  }
}

CustType* CustType::divide(CustType* t1, CustType* t2) {
  if( (t1->getType() == NUMBER) && (t2->getType() == NUMBER) ) {
    NumType *temp1 = dynamic_cast<NumType*>(t1);
    NumType *temp2 = dynamic_cast<NumType*>(t2);
    
    double num = (temp1->da) / (temp2->da);
    NumType *t = new NumType(num, NUMBER);
    return t;  
  }
  else {
    cout << "ERROR: Divide only allowed for NUMBER, NUMBER" << endl;
    return NULL;
  }
}

CustType* CustType::mod(CustType* t1 , CustType* t2) {
  if( (t1->getType() == NUMBER) && (t2->getType() == NUMBER) ) {
    NumType *temp1 = dynamic_cast<NumType*>(t1);
    NumType *temp2 = dynamic_cast<NumType*>(t2);

    int num1 = static_cast<int>(temp1->da);
    int num2 = static_cast<int>(temp2->da);

    int mod = num1 % num2;

    double num = static_cast<double>(mod);
    NumType *t = new NumType(num, NUMBER);
    return t;  
  }
  else {
    cout << "ERROR: Mod only allowed for NUMBER, NUMBER" << endl;
    return NULL;
  }
}

//Comparison Operators for NumType

CustType* operator==(CustType &lhs, CustType &rhs)
{
	CustType *t1 = (&lhs);
  	CustType *t2 = (&rhs);
  	bool tempBool ; 
  	BoolType *toReturn ;
  	if ( t1 -> getType() != t2 -> getType () ){
		//cout << "== defined on operands of same type" ;
		return (new BoolType(false, BOOL) ) ;
	}
	if (t1 -> getType() == NUMBER ){
		NumType *temp1 = dynamic_cast<NumType *>(&lhs);
		NumType *temp2 = dynamic_cast<NumType *>(&rhs);
		bool tempBool = ((temp1->da)==(temp2->da)); 
		toReturn = new BoolType(tempBool, BOOL);
		return toReturn ;	
	}
	else if (t1 -> getType() == STRING ){
		StringType *temp3 = dynamic_cast<StringType *>(&lhs);
		StringType *temp4 = dynamic_cast<StringType *>(&rhs);
		tempBool = ((temp3->da)==(temp4->da));
  		toReturn = new BoolType(tempBool, BOOL);
  		return toReturn ;	 
  	}	
	else if (t1 -> getType ()  == BOOL){
		BoolType *temp5 = dynamic_cast<BoolType *>(&lhs);
		BoolType *temp6 = dynamic_cast<BoolType *>(&rhs);
		tempBool = ((temp5->da)==(temp6->da));
  		toReturn = new BoolType(tempBool, BOOL);
  		return toReturn ;	
	}
	else if (t1 -> getType () == LIST){
		ListType *temp7 = dynamic_cast<ListType *>(&lhs);
		ListType *temp8 = dynamic_cast<ListType *>(&rhs);
		if ( (temp7 -> da).size() != (temp8 -> da).size() )
			return (new BoolType(false, BOOL) ) ; 
		for (vector<CustType*> :: iterator it1 = (temp7 -> da).begin () , it2 = (temp8 -> da).begin ()  ; it1 != (temp7 -> da).end () && it2 != (temp8 -> da).end () ; ++ it1 , ++it2) {
			(*it1) -> print() ; 
			(*it2) -> print () ; 
			BoolType* tmpResult = dynamic_cast<BoolType *> ((*(*it1)) == (*(*it2)) );
			if (tmpResult -> getBoolValue () == false)
				return (new BoolType(false, BOOL) ) ; 
		}
		return (new BoolType(true, BOOL));
  	}				
  	else if (t1 -> getType () == JSON ) {
  		JsonType *temp9 = dynamic_cast<JsonType *>(&lhs);
		JsonType *temp10 = dynamic_cast<JsonType *>(&rhs);
		vector<string> attrList1 = temp9 -> getStringAttrList () ; 
		vector<string> attrList2 = temp10 -> getStringAttrList () ; 
		if ( attrList1.size () != attrList2.size() )
			return (new BoolType(false, BOOL) ) ; 
		for ( vector<string> :: iterator it = attrList1.begin () ; it != attrList1.end () ; ++ it) {
			CustType* el2 = temp10 ->getElement(*it) ; 
			if ( el2 == NULL)
				return (new BoolType(false, BOOL) ) ; 
			CustType* el1 = temp9 -> getElement(*it) ;
			BoolType* tmpResult = dynamic_cast<BoolType *> ((*el1 == *el2)) ;
			if (tmpResult -> getBoolValue () == false)
				return (new BoolType(false, BOOL) ) ; 
		}
		return (new BoolType(true, BOOL));
  	}
  	else {
  		cout << "ERROR: TYPE NOT DEFINIED FOR OBJECT" << endl;
		return NULL;
	}
}

CustType* operator!=(CustType &lhs, CustType &rhs)
{
  NumType *temp1 = dynamic_cast<NumType *>(&lhs);
  NumType *temp2 = dynamic_cast<NumType *>(&rhs);
  
  bool tempBool = ((temp1->da)!=(temp2->da));
  BoolType *toReturn = new BoolType(tempBool, BOOL);
  
  return toReturn;
}

CustType* operator<(CustType &lhs, CustType &rhs)
{
  NumType *temp1 = dynamic_cast<NumType *>(&lhs);
  NumType *temp2 = dynamic_cast<NumType *>(&rhs);
  
  bool tempBool = ((temp1->da)<(temp2->da));
  BoolType *toReturn = new BoolType(tempBool, BOOL);
  
  return toReturn;
}

CustType* operator>(CustType &lhs, CustType &rhs)
{
  NumType *temp1 = dynamic_cast<NumType *>(&lhs);
  NumType *temp2 = dynamic_cast<NumType *>(&rhs);
  
  bool tempBool = ((temp1->da)>(temp2->da));
  BoolType *toReturn = new BoolType(tempBool, BOOL);
  
  return toReturn;
}

CustType* operator<=(CustType &lhs, CustType &rhs)
{
  NumType *temp1 = dynamic_cast<NumType *>(&lhs);
  NumType *temp2 = dynamic_cast<NumType *>(&rhs);
  
  bool tempBool = ((temp1->da)<=(temp2->da));
  BoolType *toReturn = new BoolType(tempBool, BOOL);
  
  return toReturn;
}

CustType* operator>=(CustType &lhs, CustType &rhs)
{
  NumType *temp1 = dynamic_cast<NumType *>(&lhs);
  NumType *temp2 = dynamic_cast<NumType *>(&rhs);
  
  bool tempBool = ((temp1->da)>=(temp2->da));
  BoolType *toReturn = new BoolType(tempBool, BOOL);
  
  return toReturn;
}

// Logical Operators for BoolType

CustType* operator!(CustType &term)
{
  BoolType *temp = dynamic_cast<BoolType *>(&term);

  bool tempBool = !(temp->da);
  BoolType *toReturn = new BoolType(tempBool, BOOL);
  return toReturn;
}

CustType* operator&&(CustType &t1, CustType &t2)
{
  BoolType *temp1 = dynamic_cast<BoolType *>(&t1);
  BoolType *temp2 = dynamic_cast<BoolType *>(&t2);

  bool tempBool = ((temp1->da) && (temp2->da));
  BoolType *toReturn = new BoolType(tempBool, BOOL);
  
  return toReturn;
}

CustType* operator||(CustType &t1, CustType &t2)
{
  BoolType *temp1 = dynamic_cast<BoolType *>(&t1);
  BoolType *temp2 = dynamic_cast<BoolType *>(&t2);

  bool tempBool = ((temp1->da) || (temp2->da));
  BoolType *toReturn = new BoolType(tempBool, BOOL);
  
  return toReturn;
}
//returns index of element if found, otherwise returns -1 
CustType* ListType :: findIndex (CustType* c){
		double i = 0 ; 
		for (vector<CustType*> :: iterator it = da.begin () ; it != da.end () ; ++ it) {
			if (( (*(*it)) == *c ) -> getBoolValue())
				return (new NumType(i,NUMBER)) ; 
			i = i + 1 ;   
		}
		i = -1 ; 
		return  (new NumType(i,NUMBER)) ;  
}
CustType* ListType :: minus (CustType* c){
	ListType* tmp = new ListType ; 
	for (vector<CustType*> :: iterator it = da.begin () ; it != da.end () ; ++ it) {
		cout << "hello\n" ; 
		if (! ((*(*it)) == *c)->getBoolValue() ){
			tmp -> add ((*it)) ; 
			cout << "world\n" ; 
		}
		 
	}
	return (CustType*)tmp ; 
}
CustType* ListType :: contains (CustType* c){
		
		for (vector<CustType*> :: iterator it = da.begin () ; it != da.end () ; ++ it) {
			if (( (*(*it)) == *c ) -> getBoolValue())
				return (new BoolType(true,BOOL)) ; 
			   
		}
		
		return  (new BoolType(false,BOOL)) ;  
}
