#include <hyperlinked/patches.hpp>

#include <hyperlinked/patches/generic.hpp>
#include <hyperlinked/patches/memory.hpp>
#include <hyperlinked/patches/culling.hpp>

namespace hyper
{
    void patches::init()
    {
        generic_patches::init();
        memory_patches::init();
        culling_patches::init();
    }
}
