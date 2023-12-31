#include <hyperlib/utils/utils.hpp>
#include <hyperlib/assets/geometry.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/effect.hpp>

namespace hyper
{
    geometry::model::model() : key(0u), solid(nullptr), replacement_textures(nullptr), replacement_texture_count(0u), lod_level(-1)
    {
    }

    geometry::model::model(std::uint32_t key) : solid(nullptr)
    {
        this->init(key);
    }

    geometry::model::~model()
    {
        this->uninit();
    }

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

    void geometry::model::uninit()
    {
        if (this->key != 0u || this->solid != nullptr)
        {
            this->disconnect();
        }

        if (this->replacement_textures != nullptr)
        {
            call_function<void(__cdecl*)(geometry::replacement_texture_entry*)>(0x005587D0)(this->replacement_textures); // #TODO
        }

        this->key = 0u;
        this->solid = nullptr;
        this->replacement_textures = nullptr;
        this->replacement_texture_count = 0u;
        this->lod_level = -1;
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

    bool geometry::model::is_lit() const
    {
        if (this->solid != nullptr)
        {
            return (this->solid->flag & (solid::flags::is_lit | solid::flags::want_spotlight_context)) != 0;
        }

        return false;
    }

    bool geometry::model::get_bounding_box(vector3& bbox_min, vector3& bbox_max) const
    {
        if (this->solid == nullptr)
        {
            bbox_min = vector3::zero();
            bbox_max = vector3::zero();

            return false;
        }
        else
        {
            bbox_min = this->solid->bbox_min;
            bbox_max = this->solid->bbox_max;

            return true;
        }
    }

    void geometry::model::apply_replacement_texture_table(geometry::replacement_texture_handle* handle, bool fixup)
    {
        if (this->replacement_textures != nullptr && this->replacement_texture_count != 0u)
        {
            if (fixup)
            {
                for (std::uint16_t i = 0u; i < this->replacement_texture_count; ++i)
                {
                    replacement_texture_entry& entry = this->replacement_textures[i];

                    if (entry.texture == nullptr || reinterpret_cast<intptr_t>(entry.texture) == -1 || entry.texture->key != entry.new_key)
                    {
                        entry.texture = texture::get_texture_info(entry.new_key, true, false);
                    }
                }
            }

            texture_entry* entries = this->solid->textures;

            std::uint32_t entry_count = this->solid->texture_count;

            for (std::uint32_t i = 0u; i < entry_count; ++i)
            {
                texture_entry& current = entries[i];

                for (std::uint32_t k = 0u; k < this->replacement_texture_count; ++k)
                {
                    replacement_texture_entry& entry = this->replacement_textures[k];

                    if (entry.new_key != 0u && current.key == entry.old_key)
                    {
                        handle->entry = &current;
                        handle->original = current.texture_info;

                        current.texture_info = entry.texture;

                        handle++;

                        break;
                    }
                }
            }
        }

        handle->entry = nullptr;
        handle->original = nullptr;
    }

    void geometry::model::restore_replacement_texture_table(geometry::replacement_texture_handle* handle)
    {
        while (handle->entry != nullptr)
        {
            handle++->entry->texture_info = handle->original;
        }
    }

    void geometry::platform_info::create_vertex_buffers()
    {
        shader_type last_type = static_cast<shader_type>(-1);

        for (std::uint32_t i = 0u; i < this->submesh_count; ++i)
        {
            geometry::mesh_entry& entry = this->mesh_entry_table[i];

            if (entry.type == last_type)
            {
                entry.d3d_vertex_buffer = this->mesh_entry_table[i - 1].d3d_vertex_buffer;
                entry.d3d_vertex_count = this->mesh_entry_table[i - 1].d3d_vertex_count;
            }
            else
            {
                void* buffer;

                last_type = entry.type;

                directx::device()->CreateVertexBuffer(entry.file_vertex_buffer_size, D3DUSAGE_WRITEONLY, 0u, ::D3DPOOL_MANAGED, &entry.d3d_vertex_buffer, nullptr);

                entry.d3d_vertex_count = entry.file_vertex_buffer_size / entry.effect->stride();

                if (SUCCEEDED(entry.d3d_vertex_buffer->Lock(0u, 0u, &buffer, 0u)))
                {
                    ::memcpy(buffer, entry.file_vertex_buffer, entry.file_vertex_buffer_size);

                    entry.d3d_vertex_buffer->Unlock();
                }
            }
        }

        this->are_chunks_loaded = true;
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
