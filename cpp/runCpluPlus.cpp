
#include <iostream>
#include "../cpp/cPlusPlusCompiler.h"
using namespace std;


CustType* Merge(CustType* a, CustType* b)
{
CustType* c = CustType::parse("a","STRING");
CustType* e = CustType::parse("[1,2,3]","LIST");
CustType* d = a-> getAttrList();
CustType::print(CustType::parse("hell","STRING"));
CustType::print((d)->getJoType());
for (vector<CustType*> :: iterator  loopVarattr  = d -> getListBegin () ; loopVarattr != d -> getListEnd () ;  ++loopVarattr) {
 CustType* attr = *loopVarattr;
{
CustType::print(attr);

}
}
 return c;}

CustType* MergeUtil(CustType* a, CustType* b)
{
if ((*(*((a)->getJoType()) != *(CustType::parse("Json","STRING"))) || *(*((b)->getJoType()) != *(CustType::parse("Json","STRING"))))->getBoolValue())
{
CustType::print(CustType::parse("Both the arguments must be JSON","STRING"));
 return CustType::parse("null","NULL");
} return Merge(a, b);}

int main()
{
CustType* a = CustType::parse("{\"name\":\"harsha\",\"innerJson\":{\"sub\":\"PLT\",\"mark\":[5,6,7]}}","JSON");
CustType* b = CustType::parse("{\"name\":\"harsha\",\"innerJson\":{\"sub\":\"PLT\",\"mark\":[5,6,7]}}","JSON");
CustType* c = MergeUtil(a, b);
CustType::print((c)->getJoType());
}


