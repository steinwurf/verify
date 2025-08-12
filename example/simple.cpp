
#include <iostream>
#include <verify/verify.hpp>

int main() {
    std::vector<int> v;
    VERIFY(v.size() > 0U); // expands to: std::cerr << v.size() > 0U
}
