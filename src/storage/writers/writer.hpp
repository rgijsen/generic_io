#pragma once

#include <tuple>
#include "../iterators/writer_iterator.hpp"

namespace storage
{
  template<typename... Ts>
  struct writer
  {
    using iterator_t = writer_iterator<writer<Ts...>, Ts...>;

    // for iterators which can only assign one variable
    // so multi variables are aggregated into a tuple
    virtual void write_next(const std::tuple<Ts...>& t) = 0;
    virtual void write_next(const Ts&... t) = 0;

    virtual iterator_t begin()
    {
      return iterator_t(this);
    }

    virtual iterator_t end()
    {
      return iterator_t(this);
    }
  };
}