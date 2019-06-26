
#include <algorithm>
#include "../storage/storage_types.hpp"

namespace
{
  using aggregate_type = std::pair<int, double>;

  const float floating_point_precision = 15;
  const bool binary = true;
  const bool plain_text = false;

  void write_file(std::string filename, bool format, const std::vector<aggregate_type>& outbound_data)
  {
    // writer uses raii lifetime
    auto writer = storage::make_shared_writer<int, double>(format, filename, floating_point_precision);
    std::transform(outbound_data.begin(), outbound_data.end(), writer->begin(), [](auto& x) { return std::tuple(x.first, x.second); });
  }

  auto read_file(std::string filename, bool format)
  {
    std::vector<aggregate_type> inbound_data;
    auto reader = storage::make_shared_reader<int, double>(format, filename);
    if(reader)
      std::transform(reader->begin(), reader->end(), std::back_inserter(inbound_data), [](auto& x) { return std::pair{ std::get<0>(x), std::get<1>(x) }; });
    return inbound_data;
  }
}

void RunAggregatedTypeExamples()
{
  std::vector<aggregate_type> outbound_data = 
  {
    { 12, 23.997 },
    { 567, 8901.112 },
    { 1234, 56789.445 }    
  };

  auto filename = "aggregated_type_test.txt";
  write_file(filename, plain_text, outbound_data);
  auto inbound_data = read_file(filename, plain_text);
  auto b = std::equal(outbound_data.begin(), outbound_data.end(), inbound_data.begin());
  std::cout << "plain text aggregated type io " << (b ? "succeeded": "failed") << std::endl;

  filename = "aggregated_type_test.bin";
  write_file(filename, binary, outbound_data);
  inbound_data = read_file(filename, binary);
  b = std::equal(outbound_data.begin(), outbound_data.end(), inbound_data.begin());
  std::cout << "binary aggregated type io " << (b ? "succeeded": "failed") << std::endl;
}