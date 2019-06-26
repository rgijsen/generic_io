#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iterator>

namespace string_traits
{
  inline auto split(std::string s)
  {
    std::istringstream iss(s);
    std::vector<std::string> tokens(std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>());

    return tokens;
  }

  struct string_converter
  {
    template<typename T>
    inline static void convert(const std::string s, T& a)
    {
      if constexpr(std::is_same_v<T, int> ||
                   std::is_same_v<T, long> ||
                   std::is_same_v<T, size_t>)
        a = std::stoi(s);
      else if constexpr(std::is_same_v<T, double> || std::is_same_v<T, long double>)
        a = std::stod(s);
      else if constexpr(std::is_same_v<T, std::string>)
        a = s;
    }
  };
}

namespace tuple_traits
{
  template<typename Converter, typename Vector, typename Tuple, size_t... Is>
  static auto vector_to_tuple_impl(const Vector& v, Tuple& tuple, std::index_sequence<Is...>)
  {
    ((Converter::convert(v[Is], std::get<Is>(tuple))), ...);
    return tuple;
  }

  template<typename Converter, typename Vector, typename... Ts>
  static auto vector_to_tuple(const Vector& v)
  {
    std::tuple<Ts...> tuple;
    return vector_to_tuple_impl<Converter>(v, tuple, std::make_index_sequence<sizeof...(Ts)>());
  }
}

namespace parsers
{
  // plaint text values parser
  // Note: since this is an input stream, the NoThrow does not make sense anymore as eof if simply end of stream
  template<bool NoThrow = true>
  class text_parser
  {
  private:
    const std::string separator;

    template<typename Stream, typename Head, typename... Tail>
    void invoke_unparse(Stream output_stream, Head head, Tail&&... tail) const
    {
      *output_stream << head;
      ((*output_stream << separator << tail), ...);
      *output_stream << std::endl;
    }

  public:
    static constexpr bool is_binary = false;

    text_parser(std::string sep = " ")
        : separator(sep)
    {
    }

    template<typename Stream, typename Traits, typename... Ts>
    auto parse(Stream input_stream) const
    {
      std::string line;
      std::vector<std::string> tokens;
      if(std::getline(*(input_stream.get()), line))
        tokens = string_traits::split(line);
      else
      {
        if(NoThrow)
          return Traits::template make_eof<Ts...>();
        else
          throw std::runtime_error("Error reading next line");
      }

      return tuple_traits::vector_to_tuple<string_traits::string_converter, std::vector<std::string>, Ts...>(tokens);
    }

    template<typename Stream, typename... Ts>
    void unparse(Stream output_stream, Ts&&... ts) const
    {
      static_assert(sizeof...(Ts) >= 1, "At least one value must be parsed");
      invoke_unparse(output_stream, std::forward<Ts>(ts)...);
    }
  };
}