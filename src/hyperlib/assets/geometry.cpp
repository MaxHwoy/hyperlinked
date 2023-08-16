#include <hyperlib/utils/utils.hpp>
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
            geometry::solid* solid = geometry::find_solid(key);

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

    auto geometry::find_solid(std::uint32_t key) -> geometry::solid*
    {
        return geometry::find_solid(key, nullptr);
    }

    auto geometry::find_solid(std::uint32_t key, geometry::list_header* header) -> geometry::solid*
    {
        if (!geometry::loaded_table.is_loaded(key))
        {
            return nullptr;
        }

        std::uint32_t time = utils::get_ticker();

        geometry::solid* result = nullptr;

        if (header == nullptr)
        {
            for (list_header* i = geometry::list_header_list.begin(); i != geometry::list_header_list.end(); i = i->next())
            {
                void* ptr = utils::scan_hash_table_key32(key, i->solid_index_entry_table, i->solid_count, offsetof(index_entry, key), sizeof(index_entry));

                index_entry* entry = reinterpret_cast<index_entry*>(ptr);

                if (entry != nullptr && entry->solid != nullptr)
                {
                    result = entry->solid;

                    geometry::list_header_list.move_front(i);

                    break;
                }
            }
        }
        else
        {
            void* ptr = utils::scan_hash_table_key32(key, header->solid_index_entry_table, header->solid_count, offsetof(index_entry, key), sizeof(index_entry));

            index_entry* entry = reinterpret_cast<index_entry*>(ptr);

            if (entry != nullptr)
            {
                result = entry->solid;
            }
        }

        geometry::total_find_time += utils::get_ticker_difference(time, utils::get_ticker());

        return result;
    }
}
