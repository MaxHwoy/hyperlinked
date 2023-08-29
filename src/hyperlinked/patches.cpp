#include <hyperlinked/patches.hpp>

#include <hyperlinked/patches/generic.hpp>
#include <hyperlinked/patches/memory.hpp>
#include <hyperlinked/patches/loader.hpp>
#include <hyperlinked/patches/scenery.hpp>
#include <hyperlinked/patches/sections.hpp>
#include <hyperlinked/patches/culling.hpp>
#include <hyperlinked/patches/drawing.hpp>
#include <hyperlinked/patches/streamer.hpp>

namespace hyper
{
    void patches::init()
    {
        generic_patches::init();
        memory_patches::init();
        loader_patches::init();
        scenery_patches::init();
        section_patches::init();
        culling_patches::init();
        drawing_patches::init();
        streamer_patches::init();
    }
}
