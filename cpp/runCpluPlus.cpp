
#include <iostream>
#include "../cpp/cPlusPlusCompiler.h"
using namespace std;


CustType* Merge(CustType* a, CustType* b)
{
CustType* c = CustType::parse("{}","JSON");
CustType* g = CustType::parse("[]","LIST");
CustType* f = CustType::parse("1","NUMBER");
CustType* d = a-> getAttrList();
CustType* e = b-> getAttrList();
for (vector<CustType*> :: iterator loopVarattr = (a->getAttrList ()) -> getListBegin(); loopVarattr != (a->getAttrList ()) -> getListEnd();  ++loopVarattr) {
	CustType* attr = *loopVarattr;
		{
			if ((b-> getAttrList()->contains(attr))->getBoolValue())
			{
				if ((*(*((a-> getElementByOcaml(attr))->getJoType()) == *(CustType::parse("Json","STRING"))) && *(*((b-> getElementByOcaml(attr))->getJoType()) == *(CustType::parse("Json","STRING"))))->getBoolValue())
				{
					c->addByKey(attr,Merge(a-> getElementByOcaml(attr), b-> getElementByOcaml(attr)));

				}
				else
				{
					if ((*(*((a-> getElementByOcaml(attr))->getJoType()) == *(CustType::parse("List","STRING"))) && *(*((b-> getElementByOcaml(attr))->getJoType()) == *(CustType::parse("List","STRING"))))->getBoolValue())
					{
						c->addByKey(attr,CustType::add(a-> getElementByOcaml(attr),b-> getElementByOcaml(attr)));

					}
					else
					{
						c->addByKey(attr,CustType::concat(a-> getElementByOcaml(attr),b-> getElementByOcaml(attr)));

					}
				}
			}
			else
			{
				c->addByKey(attr,a-> getElementByOcaml(attr));

			}
	}
}
for (vector<CustType*> :: iterator loopVarattr = e->getListBegin(); loopVarattr != e->getListEnd();  ++loopVarattr) {
 CustType* attr = *loopVarattr;
{
if ((!(*(d->contains(attr))))->getBoolValue())
{
c->addByKey(attr,b-> getElementByOcaml(attr));

}
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
CustType* b = CustType::parse("{\"name\":\"arpit\",\"innerJson\":{\"sub\":\"OS\",\"mark\":[7,8,9]}}","JSON");
CustType* c = MergeUtil(a, b);
CustType::print(c);
}


