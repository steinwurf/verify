// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>
#include <verify/verify.hpp>

int main()
{
    int x = 5;
    int y = 10;

    // This will pass
    VERIFY(x < y, "x should be less than y");
}
