#pragma once

#include <tuple>

namespace storage
{
  template<typename Writer, typename... Ts>
  class writer_iterator
  {
  private:
    Writer* writer;

  public:
    using iterator_t = writer_iterator<Writer, Ts...>;
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using pointer = void;
    using reference = void;

    writer_iterator(Writer* w)
        : writer(w)
    {
    }

    iterator_t& operator=(const std::tuple<Ts...>& t)
    {
      writer->write_next(t);
      return (*this);
    }

    template<typename T>
    iterator_t& operator=(const T& t)
    {
      writer->write_next(t);
      return (*this);
    }

    iterator_t& operator*()
    {
      return (*this);
    }

    iterator_t& operator++()
    { // preincrement
      return (*this);
    }

    iterator_t operator++(int)
    { // postincrement
      return (*this);
    }
  };
}