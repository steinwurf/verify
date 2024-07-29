#include "include/verify.hpp"
#include <iostream>
#include <map>
#include <string>

void zoog(const std::map<std::string, int> &map) {
  auto it = map.find("foo");
  VERIFY(it != map.cend(), "expected key not found", map, "hello", "world");
}

int main() {
  std::map<std::string, int> m{{"test", 1}};
  int a = 42;
  VERIFY(a == 21);
  VERIFY(a = 21);
  VERIFY(a == 21);
  zoog(m);
  std::cout << "Hello World!" << a << "\n";
  return 0;
}
