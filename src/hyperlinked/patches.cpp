#include <hyperlinked/patches.hpp>

#include <hyperlinked/patches/generic.hpp>

#include <hyperlinked/patches/memory/memory.hpp>

#include <hyperlinked/patches/assets/loader.hpp>
#include <hyperlinked/patches/assets/scenery.hpp>
#include <hyperlinked/patches/assets/textures.hpp>
#include <hyperlinked/patches/assets/world_anim.hpp>

#include <hyperlinked/patches/renderer/camera.hpp>
#include <hyperlinked/patches/renderer/culling.hpp>
#include <hyperlinked/patches/renderer/directx.hpp>
#include <hyperlinked/patches/renderer/effect.hpp>
#include <hyperlinked/patches/renderer/flare_renderer.hpp>
#include <hyperlinked/patches/renderer/lighting.hpp>
#include <hyperlinked/patches/renderer/renderer.hpp>
#include <hyperlinked/patches/renderer/view.hpp>
#include <hyperlinked/patches/renderer/world_renderer.hpp>

#include <hyperlinked/patches/streamer/sections.hpp>
#include <hyperlinked/patches/streamer/streamer.hpp>

#include <hyperlinked/patches/world/collision.hpp>
#include <hyperlinked/patches/world/world.hpp>

namespace hyper
{
    void patches::init()
    {
        generic_patches::init();

        memory_patches::init();
        
        loader_patches::init();
        scenery_patches::init();
        texture_patches::init();
        world_anim_patches::init();
        
        camera_patches::init();
        culling_patches::init();
        directx_patches::init();
        flare_renderer_patches::init();
        effect_patches::init();
        lighting_patches::init();
        renderer_patches::init();
        view_patches::init();
        world_renderer_patches::init();
        
        section_patches::init();
        streamer_patches::init();
        
        collision_patches::init();
        world_patches::init();
    }
}
