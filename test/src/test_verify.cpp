#include <gtest/gtest.h>
#include <map>
#include <string>
#include <verify/verify.hpp>

TEST(test_verify, simple) {
  int a = 42;
  VERIFY(a == 42);
  VERIFY(a = 21);
  VERIFY(a == 21);
}

TEST(test_verify, complex) {
  std::map<std::string, int> example_map{{"test", 1}, {"foo", 2}};
  auto it = example_map.find("foo");
  VERIFY(it != example_map.cend(), "expected key not found", example_map,
         "hello", "world");
}
