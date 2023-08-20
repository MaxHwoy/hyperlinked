#pragma once

#pragma warning (disable : 4324)
#pragma warning (disable : 26451)
#pragma warning (disable : 26495)

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

#define call_function reinterpret_cast
#define nameof(X) #X

#include <hyperlib/hook.hpp>
#include <hyperlib/math.hpp>
#include <hyperlib/time.hpp>
#include <hyperlib/array.hpp>
#include <hyperlib/chunk.hpp>
#include <hyperlib/bitset.hpp>
#include <hyperlib/random.hpp>
#include <hyperlib/string.hpp>
#include <hyperlib/hashing.hpp>
#include <hyperlib/function.hpp>
#include <hyperlib/linked_list.hpp>

#define hyper_interface struct __declspec(novtable)

#define ASSERT_SIZE(T, N) static_assert(sizeof(T) == N, "sizeof("#T") != "#N)

#define CREATE_ENUM_EXPR_OPERATORS(T)																\
	extern "C++"																					\
	{																								\
		constexpr inline T& operator++(T& a) noexcept												\
		{																							\
			return (T&)(++((std::underlying_type<T>::type&)a));										\
		}																							\
																									\
		constexpr inline T& operator--(T& a) noexcept												\
		{																							\
			return (T&)(--((std::underlying_type<T>::type&)a));										\
		}																							\
																									\
		constexpr inline T operator+(T a, T b) noexcept												\
		{																							\
			return T(((std::underlying_type<T>::type)a) + ((std::underlying_type<T>::type)b));		\
		}																							\
																									\
		constexpr inline T operator-(T a, T b) noexcept												\
		{																							\
			return T(((std::underlying_type<T>::type)a) + ((std::underlying_type<T>::type)b));		\
		}																							\
																									\
		inline T& operator+=(T& a, T b) noexcept													\
		{																							\
			return (T&)(((std::underlying_type<T>::type&)a) += ((std::underlying_type<T>::type)b));	\
		}																							\
																									\
		inline T& operator-=(T& a, T b) noexcept													\
		{																							\
			return (T&)(((std::underlying_type<T>::type&)a) -= ((std::underlying_type<T>::type)b));	\
		}																							\
	}

#define CREATE_ENUM_FLAG_OPERATORS(T)																\
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
																									\
		constexpr inline bool operator==(T a, std::int32_t b) noexcept								\
		{																							\
			return ((std::underlying_type<T>::type)a) == ((std::underlying_type<T>::type)b);		\
		}																							\
																									\
		constexpr inline bool operator!=(T a, std::int32_t b) noexcept								\
		{																							\
			return ((std::underlying_type<T>::type)a) != ((std::underlying_type<T>::type)b);		\
		}																							\
	}
