#pragma once

#include <memory>
#include <iostream>
#include "readers/file_reader.hpp"
#include "writers/file_writer.hpp"
#include "writers/terminal_writer.hpp"
#include "parsers/text_parser.hpp"
#include "parsers/binary_parser.hpp"

namespace storage
{
  using NoThrow = std::true_type;
  using Throw = std::false_type;

  struct io_traits
  {
    template<typename... Ts>
    static auto make_eof()
    {
      // note: this also works for std::string type which is 'max-ed' to empty string
      return std::make_tuple((std::numeric_limits<Ts>::max())...);
    }

    static void notify(std::string s)
    {
      std::cout << s << std::endl;
    }
  };

  // reader interface types
  template<typename... Ts>
  using reader_t = reader<io_traits, Ts...>;
  template<typename... Ts>
  using shared_reader_t = std::shared_ptr<reader_t<Ts...>>;
  // reader concrete types
  template<typename... Ts>
  using binary_file_reader_t = file_reader<parsers::binary_parser, io_traits, Ts...>;
  template<typename... Ts>
  using text_file_reader_t = file_reader<parsers::text_parser<NoThrow::value>, io_traits, Ts...>;

  // writer interface types
  template<typename... Ts>
  using writer_t = writer<Ts...>;
  template<typename... Ts>
  using shared_writer_t = std::shared_ptr<writer_t<Ts...>>;
  // writer concrete types
  template<typename... Ts>
  using binary_file_writer_t = file_writer<parsers::binary_parser, io_traits, Ts...>;
  template<typename... Ts>
  using text_file_writer_t = file_writer<parsers::text_parser<NoThrow::value>, io_traits, Ts...>;
}