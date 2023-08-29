#include <hyperlib/assets/props.hpp>

namespace hyper
{
    void props::sync_overrides()
    {
        call_function<void(__cdecl*)()>(0x0080D090)();
    }
}
