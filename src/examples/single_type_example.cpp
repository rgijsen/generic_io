
#include <algorithm>
#include <iostream>
#include "../storage/storage_types.hpp"

namespace 
{
  const float floating_point_precision = 15;

  // plaint text format writer
  void write_text_file(std::string filename, const std::vector<double>& outbound_data)
  {
    // writer uses raii lifetime
    auto writer = storage::text_file_writer_t<double>(filename, floating_point_precision);
    std::transform(outbound_data.begin(), outbound_data.end(), writer.begin(), [](auto x) { return x; });
  }

  // plaint text format reader
  auto read_text_file(std::string filename)
  {
    std::vector<double> inbound_data;
    // reader iterators return aggregated type (even if aggregated type consists of one type)
    auto reader = storage::text_file_reader_t<double>(filename);
    if(reader)
      std::transform(reader.begin(), reader.end(), std::back_inserter(inbound_data), [](auto x) { return std::get<0>(x); });
    return inbound_data;
  }
}

void RunSingleTypeExamples()
{
  std::vector<double> outbound_data = 
  {
    12.23,
    8901.112,
    1234.56789    
  };
  
  // plain text format
  auto filename = "single_type_test.txt";
  write_text_file(filename, outbound_data);
  auto inbound_data = read_text_file(filename);
  auto b = std::equal(outbound_data.begin(), outbound_data.end(), inbound_data.begin());
  std::cout << "plain text single type io " << (b ? "succeeded": "failed") << std::endl;
}