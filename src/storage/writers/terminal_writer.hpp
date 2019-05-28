#pragma once

#include <iostream>
#include "writer.hpp"

namespace storage
{
  using shared_output_stream_t = std::shared_ptr<std::ostream>;

  template<typename Parser, typename Traits, typename... Ts>
  class terminal_writer : public writer<Ts...>
  {
    private:
      Parser parser;
      shared_output_stream_t stream;

    public:
      // as the stream is std::cout do not try to delete stream during shared_ptr destruct
      terminal_writer(Parser p = Parser())
        : parser(p), stream(std::shared_ptr<shared_output_stream_t::element_type>(&std::cout, [](auto _) {}))
      {}

      void write_next(const std::tuple<Ts...>& t) override
      {
        throw "n/a";
      }

      void write_next(const Ts&... t) override
      {
        parser.unparse(stream, t...);
      }
  };
}