#pragma once

#include <memory>
#include <fstream>
#include "../iterators/reader_iterator.hpp"
#include "reader.hpp"

namespace storage
{
  using shared_input_stream_t = std::shared_ptr<std::ifstream>;

  template<typename Parser, typename Traits, typename... Ts>
  class file_reader : public reader<Traits, Ts...>
  {
  private:
    using data_t = std::tuple<Ts...>;

    shared_input_stream_t stream;
    Parser parser;

  public:
    file_reader(std::string path, Parser p = Parser())
        : parser(p)
    {
      stream = std::shared_ptr<std::ifstream>(new std::ifstream, [](auto p) { if(p->is_open()) p->close(); delete p; });

      std::ios_base::openmode mode = std::ios::in;
      if(Parser::is_binary)
        mode |= std::ios::binary;

      stream->open(path, mode);
      if(!*(this))
        Traits::notify("Unable to open file: " + path);
      else
        Traits::notify(std::string("Reading from ") + (Parser::is_binary ? "binary " : "") + "file: " + path);
    }

    operator bool() const
    {
      return stream->is_open();
    }

    void close()
    {
      if(*this)
        stream->close();
    }

    data_t read_next() const override
    {
      return parser.template parse<shared_input_stream_t, Traits, Ts...>(stream);
    }

    // overload to deviate from the file_reader Ts parameters.
    // eg. used when reading a header line in a data file
    template<typename... Types>
    auto read_next() const
    {
      return parser.template parse<shared_input_stream_t, Traits, Types...>(stream);
    }
  };
}