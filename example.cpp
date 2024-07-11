#include <iostream>
#include <map>
#include <string>
#include "include/steinwurf/verify.hpp"

void zoog(const std::map<std::string, int>& map) {
    VERIFY(map.contains("foo"), "expected key not found", map, "hello", "world");
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
