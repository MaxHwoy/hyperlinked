#include <hyperlib/assets/geometry.hpp>

namespace hyper
{
    void geometry::model::init(std::uint32_t key)
    {
        this->key = key;
        this->replacement_textures = nullptr;
        this->replacement_texture_count = 0u;
        this->lod_level = -1;

        if (key != 0u)
        {
            geometry::solid* solid = geometry::find_solid(key, nullptr);

            if (solid == nullptr)
            {
                model::unattached_list.add(this);
            }
            else
            {
                this->connect(solid);
            }
        }
    }

    void geometry::model::connect(geometry::solid* solid)
    {
        if (this->key != 0u && solid != this->solid)
        {
            this->disconnect();

            if (solid == nullptr)
            {
                model::unattached_list.add(this);
            }
            else
            {
                solid->model_list.add(this);
            }

            this->solid = solid;
        }
    }

    auto geometry::find_solid(std::uint32_t key, geometry::list_header* header) -> geometry::solid*
    {
        return call_function<geometry::solid*(__cdecl*)(std::uint32_t, geometry::list_header*)>(0x0055C830)(key, nullptr);
    }
}
