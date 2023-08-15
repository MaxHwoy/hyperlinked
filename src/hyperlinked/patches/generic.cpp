#include <hyperlib/hook.hpp>
#include <hyperlinked/patches/generic.hpp>

namespace hyper
{
    void generic_patches::init()
    {
        // bFreeTextureAssets
        hook::nop(0x007260AB, 2);

        // bFreeMeshAssets
        hook::nop(0x00725B55, 2);

        // VisibleSectionManager::Loader (unsigned char for max sections)
        hook::set<std::uint8_t>(0x007A895D, 0xB6);

        // eInitTextures (increase texture pack slot pool count)
        hook::set<std::uint32_t>(0x0055A018, 0x100);
    }
}
