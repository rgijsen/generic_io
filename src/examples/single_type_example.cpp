
#include <algorithm>
#include <iostream>
#include "../storage/storage_types.hpp"

void SingleTypeExample()
{
  std::vector<double> v_outbound = 
  {
    12.23,
    567.8901,
    1234.56789    
  };
  
  std::vector<double> v_inbound;

  ///////////////////////////////////////////////
  // plaint text format
  auto filename = "single_type_test.txt";

  { // use raii lifetime
    auto writer = storage::text_file_writer_t<double>(filename, 15);
    std::transform(v_outbound.begin(), v_outbound.end(), writer.begin(), [](auto& x) { return x; });
  }

  {
    // reader iterators return aggregated type (even if aggregatied type consists of one type)
    auto reader = storage::text_file_reader_t<double>(filename);
    if(reader)
      std::transform(reader.begin(), reader.end(), std::back_inserter(v_inbound), [](auto& input) { return std::get<0>(input); });
  }

  if(std::equal(v_outbound.begin(), v_outbound.end(), v_inbound.begin()))
    std::cout << "range io succeeded" << std::endl;
  else
    std::cout << "range io failed" << std::endl;

  ///////////////////////////////////////////////
  // binary format
  v_inbound.clear();
  filename = "single_type_test.bin";
  
  {
    auto writer = storage::binary_file_writer_t<double>(filename, 15);
    std::transform(v_outbound.begin(), v_outbound.end(), writer.begin(), [](auto& x) { return x; });
  }

  {
    // reader iterators return aggregated type (even if aggregatied type consists of one type)
    auto reader = storage::binary_file_reader_t<double>(filename);
    if(reader)
      std::transform(reader.begin(), reader.end(), std::back_inserter(v_inbound), [](auto& input) { return std::get<0>(input); });
  }

  if(std::equal(v_outbound.begin(), v_outbound.end(), v_inbound.begin()))
    std::cout << "range io succeeded" << std::endl;
  else
    std::cout << "range io failed" << std::endl;
}