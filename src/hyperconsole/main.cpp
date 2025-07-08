#include <hyperlib/shared.hpp>

int main()
{
    int a = 7;

    auto br = hyper::dbgbreak::set(&a, sizeof(a), hyper::dbgbreak::when::written);

    a = 9;

    ::printf("%d", a);

    hyper::dbgbreak::remove(br);

    return 0;
}
