#include <iostream>
#include <map>
#include <string>
#include "verify.hpp"

void zoog(const std::map<std::string, int> &map) {
  auto it = map.find("foo");
  VERIFY(it != map.cend(), "expected key not found", map, "hello", "world");
}

int main() {
  std::map<std::string, int> example_map{{"test", 1}, {"foo", 2}};
  zoog(example_map);
  int a = 42;
  VERIFY(a = 21);
  VERIFY(a == 21);
  std::cout << "Hello World!" << a << "\n";
  return 0;
}
