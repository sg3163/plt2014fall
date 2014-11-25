
#include <iostream>
#include "../cpp/cPlusPlusCompiler.h"
using namespace std;

CustType* a = CustType::parse("Hello World","STRING")
;

int main()
{
CustType::print(a);
 return 0;}


