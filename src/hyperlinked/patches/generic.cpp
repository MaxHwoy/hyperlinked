#include <hyperlib/hook.hpp>
#include <hyperlinked/patches/generic.hpp>

namespace hyper
{
    void generic_patches::init()
    {
        // LoadChunks
        hook::nop(0x006AD922, 5u);

        // UnloadChunks
        hook::nop(0x006A8449, 10u);

        // bFreeTextureAssets
        hook::nop(0x007260AB, 2);

        // bFreeMeshAssets
        hook::nop(0x00725B55, 2);

        // VisibleSectionManager::Loader (unsigned char for max sections)
        hook::set<std::uint8_t>(0x007A895D, 0xB6);

        // eInitTextures (increase texture pack slot pool count)
        hook::set<std::uint32_t>(0x0055A018, 0x100);

        // TrackStreamer::DetermineCurrentZones
        hook::set<std::uint16_t>(0x007A486B, 0xC033); // xor eax, eax

        // TrackStreamer::DetermineCurrentZones
        hook::nop(0x007A486D, 1u);

        // GManager::RefreshRandomEncounterIcons (unhardcode Silverton encounter locations)
        hook::nop(0x00634735, 2);

        // GCareer::EnableCareerBarriers (disable Silverton barriers)
        hook::set<std::uint8_t>(0x0061817C, 0xEB);
    }
}
