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

template <class Arg>
void printArguments(Arg const& arg)
{
   std::cout << arg; // Print last argument without a comma and end compile-time recursion
}

template <class Head, class... Tail>
void printArguments(Head const& head, Tail const&... tail)
{
   std::cout << head << ", ";
   printArguments(tail...);
}

// -------------------------------------------------------------------------------

template <class Head, class... Tail>
void printArgumentsConstExpr(Head const& head, Tail const&... tail)
{
   std::cout << head;
   if constexpr(sizeof...(tail) > 0)
   {
      std::cout << ", ";
      print(tail...);
   }
}

// -------------------------------------------------------------------------------

template <class Head, class... Tail>
void printIterative(Head const& head, Tail const&... tail)
{
   std::cout << head;
   (void)std::initializer_list<int>{((std::cout << ", " << tail), 0)...};
}

// -------------------------------------------------------------------------------

template <class T, class... Args>
auto pairWithRest(T const& t, Args const&... args)
{
   return std::make_tuple(std::make_pair(t, args)...);
}

template <class... Args>
auto selfCartesianProduct(Args const&... args)
{
   // Parameter packs get expanded from the innermost to the outermost
   // std::tuple_cat(pairWithRest(args, 1, "!", 5.0)...);
   // std::tuple_cat(pairWithRest(1, 1, "!", 5.0), pairWithRest("!", 1, "!", 5.0), pairWithRest(5.0, 1, "!", 5.0),);
   // This results in 3 tuples of pairs which then get concatenated via tuple_cat
   return std::tuple_cat(pairWithRest(args, args...)...);
}

#endif
