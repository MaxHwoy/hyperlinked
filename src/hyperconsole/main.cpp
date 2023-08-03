#include <iostream>

#include <hyperlib/shared.hpp>

int main()
{
    ::printf("%d\n", hyper::string::to_uint("1234567", 0u));

    return 0;
}
