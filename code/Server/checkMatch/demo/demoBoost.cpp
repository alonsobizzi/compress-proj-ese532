#include "checkmatch.h"
int main()
{
typedef boost::unordered_flat_map<std::string, int> unordered_flat_map;

unordered_flat_map map;

  map.emplace("cat", 4);
  auto [hi, mom] = map.emplace("shark", 0);
std::cout << hi->first << " => " << hi->second
          << " | inserted: " << std::boolalpha << mom << '\n';

  map.emplace("spider", 8);

auto [hello, winston] = map.emplace("shark", 5);
std::cout << hello->first << " => " << hello->second
          << " | inserted: " << std::boolalpha << winston << '\n';
          
  for (const auto &p : map)
    std::cout << p.first << ";" << p.second << '\n';

  std::cout << map.size() << '\n';
  std::cout << map.max_size() << '\n';

  std::cout << std::boolalpha << (map.find("cat") != map.end()) << '\n';
  std::cout << map.count("shark") << '\n';
}
