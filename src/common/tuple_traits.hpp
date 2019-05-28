#pragma once

#include <tuple>
#include <vector>

namespace
{
  template<typename Converter, typename Vector, typename Tuple, size_t... Is>
  static auto vector_to_tuple_impl(const Vector& v, Tuple& tuple, std::index_sequence<Is...>)
  {
    ((Converter::convert(v[Is], std::get<Is>(tuple))), ...);
    return tuple;
  }

  template<typename Converter, typename Vector, typename Tuple, size_t... Is>
  static auto tuple_to_vector_impl(const Tuple& t, std::index_sequence<Is...>)
  {
    Vector v;
    ((v.push_back(Converter::convert(std::get<Is>(t)))), ...);
    return v;
  }

  template<class F, class... Ts, std::size_t... Is>
  void tuple_for_each_impl(const std::tuple<Ts...>& tuple, F f, std::index_sequence<Is...>)
  {
    ((f(std::get<Is>(tuple))), ...);
  }

  template<class F, class... Ts, std::size_t... Is>
  void tuple_for_each_index_impl(const std::tuple<Ts...>& tuple, F f, std::index_sequence<Is...>)
  {
    ((f(std::get<Is>(tuple), Is)), ...);
  }

  template<typename OutputIterator, class F, class... Ts, std::size_t... Is>
  void tuple_transform_impl(const std::tuple<Ts...>& tuple, OutputIterator result, F f, std::index_sequence<Is...>)
  {
    (((*result = f(std::get<Is>(tuple))), ++result), ...);
  }

  template<typename InputIterator, typename OutputIterator, class F, class... Ts, std::size_t... Is>
  void tuple_transform_impl(const std::tuple<Ts...>& tuple, InputIterator first2, OutputIterator result, F f, std::index_sequence<Is...>)
  {
    (((*result = f(std::get<Is>(tuple), *first2)), ++result, ++first2), ...);
  }
}

namespace tuple_traits
{
  struct converter_nop
  {
    template<typename T>
    static auto convert(T& a) { return a; }
  };

  template<typename Converter, typename Vector, typename... Ts>
  static auto vector_to_tuple(const Vector& v)
  {
    std::tuple<Ts...> tuple;
    return vector_to_tuple_impl<Converter>(v, tuple, std::make_index_sequence<sizeof...(Ts)>());
  }

  template<typename Converter, typename Vector, typename Tuple>
  static auto tuple_to_vector(const Tuple& tuple)
  {
    return tuple_to_vector_impl<Converter, Vector>(tuple, std::make_index_sequence<std::tuple_size_v<Tuple>>());
  }

  template<class F, class... Ts>
  void tuple_for_each(const std::tuple<Ts...>& tuple, F f)
  {
    tuple_for_each_impl(tuple, f, std::make_index_sequence<sizeof...(Ts)>());
  }

  template<class F, class... Ts>
  void tuple_for_each_index(const std::tuple<Ts...>& tuple, F f)
  {
    tuple_for_each_index_impl(tuple, f, std::make_index_sequence<sizeof...(Ts)>());
  }

  template<typename OutputIterator, class F, class... Ts>
  void tuple_transform(const std::tuple<Ts...>& tuple, OutputIterator result, F f)
  {
    tuple_transform_impl(tuple, result, f, std::make_index_sequence<sizeof...(Ts)>());
  }

  template<typename InputIterator, typename OutputIterator, class F, class... Ts>
  void tuple_transform(const std::tuple<Ts...>& tuple, InputIterator first2, OutputIterator result, F f)
  {
    tuple_transform_impl(tuple, first2, result, f, std::make_index_sequence<sizeof...(Ts)>());
  }
}