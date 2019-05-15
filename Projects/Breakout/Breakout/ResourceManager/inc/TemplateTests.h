#ifndef TEMPLATE_TESTS_H
#define TEMPLATE_TESTS_H

#include <tuple>
#include <utility>
#include <iostream>

// -------------------------------------------------------------------------------

template <class... Args>
void createTuple(int i, Args... args)
{
   // Expand template parameter pack Args first, then function parameter pack args
   std::tuple<Args...> argsTuple{args...};
}

// -------------------------------------------------------------------------------

template <class... Args>
void createTupleUniRef(int i, Args&&... args)
{
   // Expand template parameter pack Args first, then function parameter pack args
   std::tuple<Args...> argsTuple{std::forward<Args>(args)...};
}

// -------------------------------------------------------------------------------

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
printTuple(std::tuple<Tp...>& t)
{
   // End compile-time recursion
}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
printTuple(std::tuple<Tp...>& t)
{
   std::cout << std::get<I>(t) << '\n';
   printTuple<I + 1, Tp...>(t);
}

// -------------------------------------------------------------------------------

#endif
