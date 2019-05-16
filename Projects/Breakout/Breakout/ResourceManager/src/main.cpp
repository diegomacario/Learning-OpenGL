#include "TemplateTests.h"

int main()
{
   createTuple(1, 2.0f, 3.0, 4u, "A");

   createTupleUniRef(1, 2.0f, 3.0, 4u, "A");

   auto t = std::make_tuple(1, 2.0f, 3.0, 4u, "A");
   printTuple(t);

   printArguments(1, 2.0f, 3.0, 4u, "A");

   int x;
   std::cin >> x;

   return 0;
}
