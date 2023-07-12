#pragma once

#include <hyperlib/shared.hpp>

namespace hyper
{
    class dal final
    {
    public:
        hyper_interface i_dal
        {
			enum class i_id : std::uint32_t
			{
				i_template,
				i_racer,
				i_vehicle,
				i_event,
				i_lang_hash,
				i_options,
				i_career,
				i_fe_vehicle,
				i_pause_states,
				i_world_map,
				i_player,
				i_pursuit,
				i_rewards,
				i_crew_member,
				i_statistics,
				i_online_database,
				i_challenge,
				i_online,
			};

			virtual ~i_dal() = default;

			virtual bool is_interface_valid(std::int32_t type, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;

			virtual bool get_floating(std::int32_t type, float& value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;
			virtual bool set_floating(std::int32_t type, float value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;

			virtual bool increment_floating(std::int32_t type, float value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;
			virtual bool decrement_floating(std::int32_t type, float value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;

			virtual bool get_integer(std::int32_t type, std::int32_t& value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;
			virtual bool set_integer(std::int32_t type, std::int32_t value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;

			virtual bool increment_integer(std::int32_t type, std::int32_t value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;
			virtual bool decrement_integer(std::int32_t type, std::int32_t value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;

			virtual bool get_string(std::int32_t type, char* value, std::int32_t size, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;
			virtual bool set_string(std::int32_t type, const char* value, std::int32_t size, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;

			virtual bool default_integer(std::int32_t type, std::int32_t& value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;
			virtual bool default_floating(std::int32_t type, float& value, std::int32_t a1, std::int32_t a2, std::int32_t a3) = 0;

			i_id id;
        };

        struct manager : public i_dal
        {
			i_dal* interfaces[0x12];
			bool is_initialized;

			static inline manager& instance = *reinterpret_cast<manager*>(0x00A8AD30);
        };
    };

	ASSERT_SIZE(dal::manager, 0x54);
}
