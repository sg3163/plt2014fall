
#include <iostream>
#include "../cpp/cPlusPlusCompiler.h"
using namespace std;

CustType* a = CustType::parse("Hello World","STRING")
;

CustType* test(CustType* b)
{
CustType::print(b);
 return CustType::parse("0","NUMBER")
;}

int main()
{
CustType::print(a);
 return 0;}


