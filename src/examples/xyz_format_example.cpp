
#include <algorithm>
#include "../storage/storage_types.hpp"

namespace
{
  const float floating_point_precision = 15;

  struct element
  {
    std::string symbol;
    double x, y, z;
  };

  bool operator==(const element& lhs, const element& rhs)
  {
    return lhs.symbol == rhs.symbol &&
            lhs.x == rhs.x &&
            lhs.y == rhs.y &&
            lhs.z == rhs.z;
  }

  auto write_file_via_iterator(std::string filename, std::vector<element> data)
  {
    auto writer = storage::text_file_writer_t<std::string, double, double, double>(filename, floating_point_precision);
    writer.write_next(data.size()); // number of atoms
    writer.write_next(std::string("Lorem Ipsum")); // comment

    std::transform(data.begin(), data.end(), writer.begin(), [](auto& el)
    { 
      // create tuple as return type to the iterator
      return std::tuple(el.symbol, el.x, el.y, el.z); 
    });
  }

  auto write_file_via_range(std::string filename, std::vector<element> data)
  {
    auto writer = storage::text_file_writer_t<std::string, double, double, double>(filename, floating_point_precision);
    writer.write_next(data.size()); // number of atoms
    writer.write_next(std::string("Lorem Ipsum")); // comment

    for(auto el : data)
    {
      // directly send data to the writer
      writer.write_next(el.symbol, el.x, el.y, el.z);
    }
  }

  auto read_file(std::string filename)
  {
    std::vector<element> data;

    auto reader = storage::text_file_reader_t<std::string, double, double, double>(filename);
    reader.read_next<int>(); // header
    reader.read_next<std::string>(); // comment
    std::transform(reader.begin(), reader.end(), std::back_inserter(data), [](auto& a)
    {
      return element{ std::get<0>(a), std::get<1>(a), std::get<2>(a), std::get<3>(a) }; 
    });

    return data;
  }
}

// the unofficial xyz format
// specified as: https://en.wikipedia.org/wiki/XYZ_file_format
void RunXYZFormatExample()
{
  std::vector<element> outbound_data = 
  {
    { "C", 1., 2., 3. },
    { "N", -0.81, 22.34, -3.321 },
    { "H", -0.1133, 0.3344, -0.32123 }
  };

  std::string filename = "xyz_format_test.xyz";

  // write via iterator
  write_file_via_iterator(filename, outbound_data);
  auto inbound_data = read_file(filename);
  auto b = std::equal(outbound_data.begin(), outbound_data.end(), inbound_data.begin());
  std::cout << "xyz format io " << (b ? "succeeded": "failed") << std::endl;

  // write via range
  write_file_via_range(filename, outbound_data);
  inbound_data = read_file(filename);
  b = std::equal(outbound_data.begin(), outbound_data.end(), inbound_data.begin());
  std::cout << "xyz format io " << (b ? "succeeded": "failed") << std::endl;
}