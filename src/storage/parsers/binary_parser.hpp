#pragma once

namespace parsers
{
  class binary_parser
  {
  private:
    template<typename Stream, typename Tuple, size_t... Is>
    auto create_tuple(Stream input_stream, Tuple tuple, std::index_sequence<Is...>) const
    {
      // fold read. Presume the data for all tuple elements is present.
      (((*input_stream).read((char*)&std::get<Is>(tuple), sizeof(std::tuple_element_t<Is, Tuple>))), ...);
      return tuple;
    }

  public:
    static constexpr bool is_binary = true;

    template<typename Stream, typename Traits, typename... Ts>
    auto parse(Stream input_stream) const
    {
      if((*input_stream).eof())
        return Traits::template make_eof<Ts...>();

      std::tuple<Ts...> tuple;
      return create_tuple(input_stream, tuple, std::make_index_sequence<sizeof...(Ts)>());
    }

    template<typename Stream, typename... Ts>
    void unparse(Stream output_stream, Ts&&... ts) const
    {
      // fold write
      (output_stream->write((char*)&ts, sizeof(ts)), ...);
    }
  };
}