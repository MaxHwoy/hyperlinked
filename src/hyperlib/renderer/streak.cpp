#include <hyperlib/assets/flares.hpp>
#include <hyperlib/renderer/streak.hpp>
#include <hyperlib/renderer/directx.hpp>
#include <hyperlib/renderer/view.hpp>
#include <hyperlib/renderer/drawing.hpp>

namespace hyper
{
    void streak::manager::initialize()
    {
        const std::uint32_t streaks = 0x80;

        this->max_streaks = streaks;
        this->streak_count = 0u;
        this->_0x18 = false;
        this->vertex_count = streaks * 4u;
        this->polies = nullptr;
        this->locked = false;

        directx::device()->CreateVertexBuffer(sizeof(poly) * streaks, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0u, D3DPOOL_DEFAULT, &this->vertex_buffer, nullptr);
        directx::device()->CreateIndexBuffer(sizeof(std::uint16_t) * 6u * streaks, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &this->index_buffer, nullptr);

        std::uint16_t* indexer;

        if (SUCCEEDED(this->index_buffer->Lock(0u, 0u, reinterpret_cast<void**>(&indexer), 0u)))
        {
            for (std::uint16_t i = 0u, k = 0u; i < streaks; ++i, k += 4u)
            {
                *indexer++ = k + 0u;
                *indexer++ = k + 1u;
                *indexer++ = k + 2u;
                *indexer++ = k + 0u;
                *indexer++ = k + 2u;
                *indexer++ = k + 3u;
            }

            this->index_buffer->Unlock();
        }

        this->flare_texture_page = texture::get_texture_info(hashing::bin("FLARE_TEXTURE_PAGE"), true, false);
        this->streak_flares_texture = texture::get_texture_info(hashing::bin("STREAKFLARES_I"), true, false);
        this->flare_texture_page->pinfo->state.colour_write_alpha = false;
    }

    void streak::manager::destroy()
    {
        if (this->vertex_buffer != nullptr)
        {
            this->vertex_buffer->Release();
            this->vertex_buffer = nullptr;
        }

        if (this->index_buffer != nullptr)
        {
            this->index_buffer->Release();
            this->index_buffer = nullptr;
        }
    }

    void streak::manager::lock()
    {
        HRESULT result = this->vertex_buffer->Lock(0u, 0u, reinterpret_cast<void**>(&this->polies), D3DLOCK_DISCARD);

        assert(SUCCEEDED(result));

        this->streak_count = 0u;
        this->_0x18 = true;
        this->locked = true;
    }

    void streak::manager::unlock()
    {
        if (this->_0x18 && this->vertex_buffer != nullptr)
        {
            this->_0x18 = false; // whyyyyy do we even neeeeed this thing???
        }

        this->vertex_buffer->Unlock();
        this->polies = nullptr;
        this->locked = false;
    }

    void streak::manager::render(render_view* view, void* flush_fac)
    {
        // #TODO
    }

    void streak::manager::commit_flare(const vector3& position, const texture::info* texture, flare::type type, color32 color, float horizontal_scale, float vertical_scale, float degree_angle)
    {
        if (this->locked && this->streak_count < this->max_streaks)
        {
            poly& rect = this->polies[this->streak_count++];

            float radian_angle = degree_angle * 0.017453292f;

            float brightness = (type == flare::type::lamppost || type >= flare::type::blinking_amber) ? 1.0f : 0.0f;

            const texture::page_range* range = texture::get_texture_page_range(texture->key, -1);

            rect.vertices[0].position = position;
            rect.vertices[1].position = position;
            rect.vertices[2].position = position;
            rect.vertices[3].position = position;

            rect.vertices[0].color = color;
            rect.vertices[1].color = color;
            rect.vertices[2].color = color;
            rect.vertices[3].color = color;

            rect.vertices[0].uv.x = range->u0;
            rect.vertices[1].uv.x = range->u1;
            rect.vertices[2].uv.x = range->u1;
            rect.vertices[3].uv.x = range->u0;

            rect.vertices[0].uv.y = range->v0;
            rect.vertices[1].uv.y = range->v0;
            rect.vertices[2].uv.y = range->v1;
            rect.vertices[3].uv.y = range->v1;

            rect.vertices[0].vector.x = -horizontal_scale;
            rect.vertices[1].vector.x = +horizontal_scale;
            rect.vertices[2].vector.x = +horizontal_scale;
            rect.vertices[3].vector.x = -horizontal_scale;

            rect.vertices[0].vector.y = +vertical_scale;
            rect.vertices[1].vector.y = +vertical_scale;
            rect.vertices[2].vector.y = -vertical_scale;
            rect.vertices[3].vector.y = -vertical_scale;

            rect.vertices[0].vector.z = radian_angle;
            rect.vertices[1].vector.z = radian_angle;
            rect.vertices[2].vector.z = radian_angle;
            rect.vertices[3].vector.z = radian_angle;

            rect.vertices[0].vector.w = brightness;
            rect.vertices[1].vector.w = brightness;
            rect.vertices[2].vector.w = brightness;
            rect.vertices[3].vector.w = brightness;

            rect.vertices[0].adder = 0.0f;
            rect.vertices[1].adder = 0.0f;
            rect.vertices[2].adder = 0.0f;
            rect.vertices[3].adder = 0.0f;
        }
    }
}
