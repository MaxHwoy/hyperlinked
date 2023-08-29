#include <hyperlib/utils/utils.hpp>
#include <hyperlib/assets/geometry.hpp>

namespace hyper
{
    void geometry::model::init(std::uint32_t hash_key)
    {
        this->key = hash_key;
        this->replacement_textures = nullptr;
        this->replacement_texture_count = 0u;
        this->lod_level = -1;

        if (hash_key != 0u)
        {
            geometry::solid* found = geometry::find_solid(hash_key);

            if (found == nullptr)
            {
                model::unattached_list.add(this);
            }
            else
            {
                this->connect(found);
            }
        }
    }

    void geometry::model::connect(geometry::solid* solid_to_connect)
    {
        if (this->key != 0u && solid_to_connect != this->solid)
        {
            this->disconnect();

            if (solid_to_connect == nullptr)
            {
                model::unattached_list.add(this);
            }
            else
            {
                solid_to_connect->model_list.add(this);
            }

            this->solid = solid_to_connect;
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
            for (list_header* i = geometry::list_header::list.begin(); i != geometry::list_header::list.end(); i = i->next())
            {
                void* ptr = utils::scan_hash_table_key32(key, i->solid_index_entry_table, i->solid_count, offsetof(index_entry, key), sizeof(index_entry));

                index_entry* entry = reinterpret_cast<index_entry*>(ptr);

                if (entry != nullptr && entry->solid != nullptr)
                {
                    result = entry->solid;

                    geometry::list_header::list.move_front(i);

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
