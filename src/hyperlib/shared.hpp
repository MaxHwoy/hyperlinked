#pragma once

#pragma warning (disable : 26495)
#pragma warning (disable : 4324)

#include <cstdint>
#include <cassert>

#include <d3dx9.h>
#include <d3d9.h>

#if defined(hyper)
#undef hyper
#endif

#if defined(far)
#undef far
#endif

#if defined(near)
#undef near
#endif

#include <hyperlib/hook.hpp>
#include <hyperlib/math.hpp>
#include <hyperlib/array.hpp>
#include <hyperlib/chunk.hpp>
#include <hyperlib/hashing.hpp>
#include <hyperlib/linked_list.hpp>

#define ASSERT_SIZE(T, N) static_assert(sizeof(T) == N, "sizeof("#T") != "#N)

#if !defined(DEFINE_ENUM_FLAG_OPERATORS)
#define DEFINE_ENUM_FLAG_OPERATORS(T)																\
	extern "C++"																					\
	{																								\
		constexpr inline T operator|(T a, T b) noexcept												\
		{																							\
			return T(((std::underlying_type<T>::type)a) | ((std::underlying_type<T>::type)b));		\
		}																							\
																									\
		inline T& operator|=(T& a, T b) noexcept													\
		{																							\
			return (T&)(((std::underlying_type<T>::type&)a) |= ((std::underlying_type<T>::type)b));	\
		}																							\
																									\
		constexpr inline T operator&(T a, T b) noexcept												\
		{																							\
			return T(((std::underlying_type<T>::type)a) & ((std::underlying_type<T>::type)b));		\
		}																							\
																									\
		inline T& operator&=(T& a, T b) noexcept													\
		{																							\
			return (T&)(((std::underlying_type<T>::type&)a) &= ((std::underlying_type<T>::type)b));	\
		}																							\
																									\
		constexpr inline T operator~(T a) noexcept													\
		{																							\
			return T(~((std::underlying_type<T>::type)a));											\
		}																							\
																									\
		constexpr inline T operator^(T a, T b) noexcept												\
		{																							\
			return T(((std::underlying_type<T>::type)a) ^ ((std::underlying_type<T>::type)b));		\
		}																							\
																									\
		inline T& operator^=(T& a, T b) noexcept													\
		{																							\
			return (T&)(((std::underlying_type<T>::type&)a) ^= ((std::underlying_type<T>::type)b));	\
		}																							\
	}
#endif
