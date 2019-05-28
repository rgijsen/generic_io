#pragma once

#include <tuple>

namespace storage
{
  template<typename Reader, typename... Ts>
  class reader_iterator
  {
    private:
      using data_t = std::tuple<Ts...>;

      Reader* reader;
      data_t data;

    public:
      using iterator_t = reader_iterator<Reader, Ts...>;
      using iterator_category =	std::input_iterator_tag;
      using value_type = data_t;
      using pointer = const data_t*;
      using reference = const data_t&;

      reader_iterator(data_t eof)
      {
        data = eof;
      }

      reader_iterator(Reader* r)
        : reader(r)
      {
        data = reader->read_next();
      }

      reference operator*() const
      {
        return data;
      }

      pointer operator->() const
      {	// return pointer to class object
        return (std::pointer_traits<pointer>::pointer_to(**this));
      }

      const iterator_t& operator++()
      {	// preincrement
        data = reader->read_next();
        return (*this);
      }

      const iterator_t operator++(int) 
      {	// postincrement
        data = reader->read_next();
        return (*this);
      }

      bool operator==(const iterator_t& right) const
      {
        return data == right.data;
      }

      bool operator!=(const iterator_t& right) const
      {
        return (!(*this == right));
      }
  };  
}