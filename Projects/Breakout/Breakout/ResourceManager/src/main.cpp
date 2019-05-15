#include <tuple>
#include <iostream>

#include "TemplateTests.h"

int main()
{
   createTuple(1, 2.0f, 3.0, 4u, "A");
   createTupleUniRef(1, 2.0f, 3.0, 4u, "A");
   return 0;
}
