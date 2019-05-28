#pragma once

#include <tuple>
#include "../iterators/reader_iterator.hpp"

namespace storage
{
  template<typename Traits, typename... Ts>
  struct reader
  {
    using iterator_t = reader_iterator<const reader<Traits, Ts...>, Ts...>;

    virtual std::tuple<Ts...> read_next() const = 0;
   
    virtual iterator_t begin() const
    {
      return iterator_t(this);
    }

    virtual iterator_t end() const
    {
      return iterator_t(Traits::template make_eof<Ts...>());
    }
  };
}