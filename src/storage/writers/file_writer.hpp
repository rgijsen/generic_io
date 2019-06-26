#pragma once

#include <memory>
#include <fstream>
#include <iomanip>
#include "../iterators/writer_iterator.hpp"
#include "writer.hpp"

namespace storage
{
  using shared_of_stream_t = std::shared_ptr<std::ofstream>;

  template<typename Parser, typename Traits, typename... Ts>
  class file_writer : public writer<Ts...>
  {
  private:
    std::string path;
    shared_of_stream_t stream;
    Parser parser;

    template<size_t... Is>
    void invoke_unparse(const std::tuple<Ts...> t, std::index_sequence<Is...>)
    {
      parser.unparse(stream, std::get<Is>(t)...);
    }

  public:
    file_writer(std::string file_path, const short precision = 6, Parser p = Parser())
        : path(file_path), parser(p)
    {
      stream = shared_of_stream_t(new std::ofstream, [](auto p) { if(p->is_open()) p->close(); delete p; });

      std::ios_base::openmode mode = std::ios::out;
      if(Parser::is_binary)
        mode |= std::ios::binary;

      stream->open(path, mode);
      if(!*(this))
        Traits::notify("Unable to open file: " + path);
      else
      {
        *stream << std::setprecision(precision);
        Traits::notify(std::string("Writing to ") + (Parser::is_binary ? "binary " : "") + "file: " + path);
      }
    }

    operator bool() const
    {
      return stream->is_open();
    }

    std::string get_path()
    {
      return path;
    }

    // for iterators which can only assign one variable
    // so multi variables are aggregated into a tuple
    void write_next(const std::tuple<Ts...>& t) override
    {
      invoke_unparse(t, std::make_index_sequence<sizeof...(Ts)>{});
    }

    void write_next(const Ts&... t) override
    {
      parser.unparse(stream, t...);
    }
  };
} // namespace storage