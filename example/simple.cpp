
#include <iostream>
#include <verify/verify.hpp>

int main()
{
    int x = 5;
    int y = 10;

    // This will pass
    VERIFY(x < y, "x should be less than y");
}
