
#include <iostream>
#include "../cpp/cPlusPlusCompiler.h"
using namespace std;


int main()
{
CustType* a = CustType::parse("[1,\"2\",[4,[\"qw\",3,4],\"name\"],{\"name\":\"harsha\",\"innerJson\":{\"sub\":\"PLT\",\"mark\":[5,6,7]},\"number\":2324}]","LIST")
;
CustType* b = CustType::parse("{\"name\":\"harsha\",\"innerJson\":{\"sub\":\"PLT\",\"mark\":[5,6,7]}}","JSON")
;
CustType* c = a[2];
;
CustType* d = b["name"];
;
 return 0;}


