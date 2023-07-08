#pragma once

#include <cstdint>
#include <chrono>

namespace hyper
{
	class time final
	{
	public:
		using point_type = std::chrono::system_clock::time_point;
		using result_type = std::chrono::system_clock::duration;

		static inline auto now() -> time::point_type
		{
			return std::chrono::system_clock::now();
		}

		static inline auto since(const time::point_type& point) -> time::result_type
		{
			return time::now() - point;
		}

		static inline auto since_start() -> time::result_type
		{
			return time::since(time::start_);
		}

		static inline auto to_nanoseconds(const time::result_type& value) -> std::chrono::nanoseconds::rep
		{
			return time::to<std::chrono::nanoseconds>(value);
		}

		static inline auto to_milliseconds(const time::result_type& value) -> std::chrono::milliseconds::rep
		{
			return time::to<std::chrono::milliseconds>(value);
		}

		static inline auto to_seconds(const time::result_type& value) -> std::chrono::milliseconds::rep
		{
			return time::to<std::chrono::seconds>(value);
		}

		static inline auto to_minutes(const time::result_type& value) -> std::chrono::milliseconds::rep
		{
			return time::to<std::chrono::minutes>(value);
		}

		static inline auto to_hours(const time::result_type& value) -> std::chrono::milliseconds::rep
		{
			return time::to<std::chrono::hours>(value);
		}

	private:
		template <typename T> static inline auto to(const time::result_type& value) -> typename T::rep
		{
			return std::chrono::duration_cast<T>(value).count();
		}

		static inline time::point_type start_ = time::now();
	};

	class timer final
	{
	public:
		inline timer() : start_(time::now())
		{

		}

		inline auto elapsed() const -> time::result_type
		{
			return time::now() - this->start_;
		}

		inline void reset()
		{
			this->start_ = time::now();
		}

	private:
		time::point_type start_;
	};
}
