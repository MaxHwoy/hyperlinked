#pragma once

#include <cstdint>
#include <random>

namespace hyper
{
	class random final
	{
	public:
		template <typename T> using enable_if_integral = typename std::enable_if<std::is_integral<T>::value>::type;
		template <typename T> using enable_if_floating = typename std::enable_if<std::is_floating_point<T>::value>::type;
		template <typename T> using enable_if_string = typename std::enable_if<std::is_same<T, std::string>::value>::type;

		template <typename T, typename random::enable_if_integral<T>* = nullptr> static inline auto get(T min, T max) -> T
		{
			return std::uniform_int_distribution<T>(min, max)(random::engine_);
		}

		template <typename T, typename random::enable_if_floating<T>* = nullptr> static inline auto get(T min, T max) -> T
		{
			return std::uniform_real_distribution<T>(min, max)(random::engine_);
		}

	private:
		static inline std::seed_seq seed_
		{
			std::random_device()(),
			std::random_device()(),
			std::random_device()(),
			std::random_device()(),
			std::random_device()(),
			std::random_device()(),
			std::random_device()(),
			std::random_device()(),
			std::random_device()(),
			std::random_device()(),
		};

		static inline std::mt19937 engine_ = std::mt19937(random::seed_);
	};
}