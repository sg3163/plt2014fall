#include <iostream>
#include <fstream>
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
void this_is_not_done() { 
	cout << "arpit " ; 
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
	static CustType* read(string filename);
	static void print(CustType* data) ;
	static void print (vector<CustType*>  :: iterator it ) ;
	static void write(CustType* data, string filename);
	static CustType* typeStruct(CustType* input);
	static CustType* typeStructList(CustType* input);
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
	virtual CustType* getElementByOcaml ( CustType* key ) {
		cout << "In CustType, apparently not in JSON or LIST. Calling from some other type\n" ; 
		return  NULL ; 
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
	virtual void addByKey (CustType* index , CustType* el){
		cout << "In CustType, apparently not in LIST or JSON. Calling from some other type\n" ; 
	}

	//add item to map
	virtual void add (string  key, CustType* el) { 
		cout << "In CustType, apparently not in JSON. Calling from some other type\n" ; 

	}
	/*virtual void addToJson (CustType* key, CustType* el) { 
		cout << "In CustType, apparently not in JSON. Calling from some other type\n" ; 
	}*/
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
	virtual CustType* getJoType() {
				cout << "Printing in CustType, Ooops!\n Somebody needs to implement this in child class\n" ;
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
	CustType* getJoType() {
		return ( CustType :: parse ("Bool" , "STRING")) ;
	}
	void print () {
	  if(da) { cout << "true" ; }
	  else { cout << "false" ; }
	}
	string toString () { 
		string ret = "false"; 
		if ( da)
			ret = "true" ;
		return ret ; 
	}
	CustType* makeString () {
		string ret = "false"; 
		if ( da)
			ret = "true" ;
		return CustType :: parse (this -> toString() , "STRING") ; 
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
	CustType* getJoType() {
		return ( CustType :: parse ("Number" , "STRING")) ;
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
	CustType* getJoType() {
		return (CustType :: parse ("String" , "STRING")) ;
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
	CustType* getJoType() {
		return ( CustType :: parse ("List" , "STRING")) ;
	}
	void print () {
		/*
		for (vector<CustType*> :: iterator it = da.begin () ; it != da.end () ; ++ it) {
			(*it) -> print () ;
			if ( it != (da.end() - 1) ) {
			  cout << ",";
			}
		} */
		cout << toString () ; 
	}
	string toString () { 
		string ret  = "[" ; 
		for (vector<CustType*> :: iterator it = da.begin () ; it != da.end () ; ++ it) {
			if ( it != da.begin () )
				ret += "," ; 
			ret += (*it) -> toString () ;

		}
		ret += "]" ; 
		return ret ; 

	}
	


	CustType* makeString () {
		
		return CustType :: parse(this -> toString() , "STRING" ) ; 
		
	}
	CustType* getElementByOcaml (CustType* indexNumType) {
		NumType* indexNum = dynamic_cast<NumType *>(indexNumType);
		double doubleKey = indexNum -> da ; 
		int index = (int) doubleKey ;
		if ( index >= da.size())
			return NULL ; 
		return da[index] ; 
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
	void addByKey (CustType* index , CustType* el){
		NumType* indexNum = dynamic_cast<NumType *>(index);
		double doubleKey = indexNum -> da ; 
		int intKey = (int) doubleKey ;
		if ( intKey >= 0 ){
			da [intKey]  = el ;
		}
		else {
			cout << "index has value < 0" ;
			exit (1) ;
		}
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

    void print() {/*
      JSONValue *value = new JSONValue(data);
      print_out(value->Stringify().c_str());*/
      cout <<  prettyPrint(0) ; 
    }
    CustType* getJoType() {
		return ((CustType :: parse ("Json" , "STRING")) ) ;
	}
    string toString () {
    	string ret = "{" ; 
    	for ( map<string , CustType* > :: iterator it = (this -> da).begin() ; it != (this -> da).end() ; ++ it ) { 
    		if ( it != (this -> da).begin() )
    			ret += "," ; 
    		ret += "\"" ;
    		ret += it -> first ; 
    		ret += "\"" ;

    		ret += ":" ;
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
    	ret += "}" ;
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

    		el += ":" ;
    		if ( (it -> second )  -> getType() == NUMBER || (it -> second )  -> getType() == BOOL ){
				el +=  (it -> second ) -> toString ();
			}
			else if ((it -> second ) -> getType () == STRING){
				el +=  "\"" + (it -> second ) -> toString () + "\"";
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

    		if ( (*it) -> getType() == NUMBER || (*it) -> getType() == BOOL){
				el += (*it) -> toString ();
			}
			else if ( (*it) -> getType () == STRING){
				el +=  "\"" + (*it) -> toString () + "\"";
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
	CustType * getElementByOcaml (CustType* keyStrType) {
		StringType *keyStr = dynamic_cast<StringType *>(keyStrType);
		string key = keyStr -> toString () ;
		if ( da.find(key) == da.end())
			return NULL ;
		else
			return da[key] ;  
	}
	CustType * getElement (string key) {
		if ( da.find(key) == da.end())
			return NULL ;
		else
			return da[key] ;  
	}
	void addToJson (CustType* key, CustType* el){
		StringType *keyStr = dynamic_cast<StringType *>(key);
		string stringKey = keyStr -> toString () ; 
		da [stringKey] = el ;
	}
	void addByKey (CustType* keyStrType, CustType* el) {
		StringType *keyStr = dynamic_cast<StringType *>(keyStrType);
		string key = keyStr -> toString () ;
		da [key] = el ; 
	}
	void add (string  key, CustType* el) { 
		da [key] = el ; 
	}
	CustType* minus (CustType* c){
		string k = c -> toString () ; 
		CustType* t = new JsonType ;
		for ( map<string , CustType* > ::iterator iter  =  getBeginIterator() ; iter !=  getEndIterator () ; iter ++ ) {
		
			if ( k != iter -> first)
				t -> add (iter -> first,iter -> second) ; 
			
		}
		return t ; 
	}
	CustType* contains (CustType* c){
		if (c ->getType() != STRING){
			cout << "\n JSON :: contains(key) expects a STRING argument\n" ;
			return this ; 
		}
		string k = c -> toString() ; 
		for ( map<string , CustType* > ::iterator iter  =  getBeginIterator() ; iter !=  getEndIterator () ; iter ++ ) {
		
			if ( iter -> first == k)
				return (new BoolType(true,BOOL))  ; 
			
		}
		return (new BoolType(false,BOOL))  ; 

	}

};
/******************************************************
		CLASS DEFINITIONS END HERE 
******************************************************/

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
	BoolType * t = new BoolType ( data == "true" ? 1 : 0 , BOOL ) ; 
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
		else if (type == "NUMBER"){
			//CustType :: dt = NUMBER ; 
			return getNum (data, NUMBER) ;
		}
		else {
			return NULL ; 
		}
}

void CustType :: print ( CustType* data) { 
	//cout << "I am here bitch" << endl ; 
	data -> print () ;
}

void CustType :: write (CustType * data, string filename) {
  string toWrite = data -> toString();
  
  ofstream file(filename.c_str(), ios::app);
  if ( file.is_open() )
    {
      file << toWrite << endl;
    }
  else
    {
      cout << "Unable to open file" << endl;
    }
  
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


CustType* CustType::typeStruct(CustType *input)
{
  JsonType *inputJson = dynamic_cast<JsonType *>(input);
  CustType *returnString = new StringType("{", STRING);
  CustType *element;
  
  for( map<string, CustType*> :: iterator it = (inputJson->da).begin() ; it != (inputJson->da).end() ; ++ it)
    {
      int type = (it->second)->getType();
      if ( type == NUMBER )
	{
	  element = new StringType(" String : Number, ", STRING);
	}
      else if ( type == STRING )
	{
	  element = new StringType(" String : String, ", STRING);
	}
      else if ( type == BOOL )
	{
	  element = new StringType(" String : Boolean, ", STRING);
	}
      else if ( type == JSON )
	{
	  CustType *e1 = new StringType(" String : ", STRING);
	  CustType *e2 = CustType::typeStruct(it->second);
	  CustType *e3 = new StringType(", ", STRING);
	  element = CustType::add(e1, e2);
	  element = CustType::add(element, e3);
	}
      else if ( type == LIST )
	{
	  //CustType *e1 = CustType::typeStructList(it->second);
	  //CustType *e2 = new StringType(", ", STRING);
	  //element = CustType::add(e1, e2);

	  //Use next line instead of above 3 if desired behavior is to simply return "List" instead of the types contained within the list.
	  element = new StringType(" String : List, ", STRING);
	}

      returnString = CustType::add(returnString, element);
    }

  StringType *str = dynamic_cast<StringType *>(returnString);
  string data = str->da;
  str->da = data.substr(0, data.size()-2);

  StringType *s = new StringType(" }", STRING);
  CustType *toReturn = CustType::add(str, s);

  return toReturn;
}

CustType* CustType::typeStructList(CustType *input)
{
  ListType *inputList = dynamic_cast<ListType *>(input);
  CustType *returnString = new StringType("[", STRING);
  CustType *element;
  
  for (vector<CustType*> :: iterator it = (inputList->da).begin () ; it != (inputList->da).end () ; ++ it) 
    {
      int type = (*it)->getType();
      if ( type == NUMBER )
	{
	  element = new StringType(" Number,", STRING);
	}
      else if ( type == STRING )
	{
	  element = new StringType(" String,", STRING);
	}
      else if ( type == BOOL )
	{
	  element = new StringType("Boolean, ", STRING);
	}
      else if ( type == JSON )
	{
	  CustType *e0 = new StringType(" ", STRING);
	  CustType *e1 = CustType::typeStruct(*it);
	  CustType *e2 = new StringType(", ", STRING);
	  element = CustType::add(e0, e1);
	  element = CustType::add(element, e2);
	}
      else if ( type == LIST )
	{
	  CustType *e1 = CustType::typeStructList(*it);
	  CustType *e2 = new StringType(", ", STRING);
	  element = CustType::add(e1, e2);
	}

      returnString = CustType::add(returnString, element);
    }

  StringType *str = dynamic_cast<StringType *>(returnString);
  string data = str->da;
  str->da = data.substr(0, data.size()-2);

  StringType *s = new StringType(" ]", STRING);
  CustType *toReturn = CustType::add(str, s);

  return toReturn;
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
  else if( (t1->getType() == LIST) && (t2->getType() == LIST) ) {
    ListType *temp1 = dynamic_cast<ListType*>(t1);
    ListType *temp2 = dynamic_cast<ListType*>(t2);
    ListType* t = new ListType() ; 
    for (vector<CustType*> :: iterator it = (temp1->da).begin () ; it != (temp1->da).end () ; ++ it) {
			t -> add (*it) ; 
	}
	for (vector<CustType*> :: iterator it = (temp2->da).begin () ; it != (temp2->da).end () ; ++ it) {
			t -> add (*it) ; 
	}
    
    return t;
  }
  else {
    cout << "ERROR: ++ only allowed for NUMBER, NUMBER or STRING, STRING or LIST,LIST" << endl;
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
		//cout << "\nprinting inside c ++ " << temp5 -> getBoolValue() << temp6 -> getBoolValue() << tempBool << endl ; 
  		toReturn = new BoolType(tempBool, BOOL);
  		return toReturn ;	
	}
	else if (t1 -> getType () == LIST){
		ListType *temp7 = dynamic_cast<ListType *>(&lhs);
		ListType *temp8 = dynamic_cast<ListType *>(&rhs);
		if ( (temp7 -> da).size() != (temp8 -> da).size() )
			return (new BoolType(false, BOOL) ) ; 
		for (vector<CustType*> :: iterator it1 = (temp7 -> da).begin () , it2 = (temp8 -> da).begin ()  ; it1 != (temp7 -> da).end () && it2 != (temp8 -> da).end () ; ++ it1 , ++it2) {
			//(*it1) -> print() ; 
			//(*it2) -> print () ; 
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
  return (new BoolType( !( (lhs == rhs) ->getBoolValue() ), BOOL ) ); ; 

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

// Reads from text file. Returns a ListType
CustType* CustType::read(string filename)
{
  string fileText = "";
  string line;
  ifstream file (filename.c_str());
  if ( file.is_open() )
    {
      while ( getline(file, line) )
	{
	  fileText+=line;
	}
      file.close();
    }
  else { cout << "Unable to open file" << endl; }

  CustType *toReturn = CustType::parse(fileText, "JSON");
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
		
		if (! ((*(*it)) == *c)->getBoolValue() ){
			tmp -> add ((*it)) ; 
			
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