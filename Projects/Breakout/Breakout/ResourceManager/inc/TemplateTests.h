#ifndef TEMPLATE_TESTS_H
#define TEMPLATE_TESTS_H

#include <tuple>
#include <iostream>

template <class... Args>
void createTuple(int i, Args... args)
{
   // Expand template parameter pack Args first, then function parameter pack args
   std::tuple<Args...> argsTuple{args...};
}

template <class... Args>
void createTupleUniRef(int i, Args&&... args)
{
   // Expand template parameter pack Args first, then function parameter pack args
   std::tuple<Args...> argsTuple{std::forward<Args>(args)...};
}

#endif
