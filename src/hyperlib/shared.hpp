#pragma once

#pragma warning (disable : 4324)
#pragma warning (disable : 26439)
#pragma warning (disable : 26451)
#pragma warning (disable : 26495)

#include <cstdint>

#define call_function reinterpret_cast
#define nameof(X) #X

#include <hyperlib/platform.hpp>

#if defined(PLATFORM_WINDOWS)
#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#endif

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
#include <hyperlib/time.hpp>
#include <hyperlib/array.hpp>
#include <hyperlib/bench.hpp>
#include <hyperlib/chunk.hpp>
#include <hyperlib/bitset.hpp>
#include <hyperlib/random.hpp>
#include <hyperlib/string.hpp>
#include <hyperlib/hashing.hpp>
#include <hyperlib/function.hpp>
#include <hyperlib/linked_list.hpp>

#define hyper_interface struct __declspec(novtable)

#define ASSERT_SIZE(T, N) static_assert(sizeof(T) == N, "sizeof("#T") != "#N)

#include <hyperlib/utils/logging.hpp>

#if defined(PLATFORM_WINDOWS)
#define PLATFORM_SHOW_ERROR(MESSAGE) ::MessageBoxA(nullptr, MESSAGE, PRODUCT_NAME, MB_ICONERROR | MB_TOPMOST)
#else
#define PLATFORM_SHOW_ERROR(MESSAGE) PRINT_FATAL("%s", MESSAGE)
#endif

#if !defined(BUILD_PRODUCTION)
#define ASSERT_WITH_MESSAGE(CONDITION, MESSAGE)	    \
		do											\
		{											\
			if (!(CONDITION))						\
			{										\
				PRINT_FATAL(MESSAGE);				\
				PLATFORM_SHOW_ERROR(MESSAGE);		\
				::__debugbreak();					\
			}										\
		}											\
													\
		while (false)

#define ASSERT(CONDITION) ASSERT_WITH_MESSAGE(CONDITION, "Assertion " #CONDITION " failed on line " nameof(__LINE__) " in file " __FILE__ ".")
#else
#define ASSERT_WITH_MESSAGE(...)
#define ASSERT(...)
#endif

inline bool assert_return(bool result)
{
	ASSERT(result);
	return result;
}

#define CREATE_ENUM_EXPR_OPERATORS(T)																	\
	extern "C++"																						\
	{																									\
		constexpr inline T& operator++(T& lhs) noexcept													\
		{																								\
			return (T&)(++((std::underlying_type<T>::type&)lhs));										\
		}																								\
																										\
		constexpr inline T& operator--(T& lhs) noexcept													\
		{																								\
			return (T&)(--((std::underlying_type<T>::type&)lhs));										\
		}																								\
																										\
		constexpr inline T operator+(T lhs, T rhs) noexcept												\
		{																								\
			return T(((std::underlying_type<T>::type)lhs) + ((std::underlying_type<T>::type)rhs));		\
		}																								\
																										\
		constexpr inline T operator-(T lhs, T rhs) noexcept												\
		{																								\
			return T(((std::underlying_type<T>::type)lhs) + ((std::underlying_type<T>::type)rhs));		\
		}																								\
																										\
		inline T& operator+=(T& lhs, T rhs) noexcept													\
		{																								\
			return (T&)(((std::underlying_type<T>::type&)lhs) += ((std::underlying_type<T>::type)rhs));	\
		}																								\
																										\
		inline T& operator-=(T& lhs, T rhs) noexcept													\
		{																								\
			return (T&)(((std::underlying_type<T>::type&)lhs) -= ((std::underlying_type<T>::type)rhs));	\
		}																								\
	}

#define CREATE_ENUM_FLAG_OPERATORS(T)																	\
	extern "C++"																						\
	{																									\
		constexpr inline T operator|(T lhs, T rhs) noexcept												\
		{																								\
			return T(((std::underlying_type<T>::type)lhs) | ((std::underlying_type<T>::type)rhs));		\
		}																								\
																										\
		inline T& operator|=(T& lhs, T rhs) noexcept													\
		{																								\
			return (T&)(((std::underlying_type<T>::type&)lhs) |= ((std::underlying_type<T>::type)rhs));	\
		}																								\
																										\
		constexpr inline T operator&(T lhs, T rhs) noexcept												\
		{																								\
			return T(((std::underlying_type<T>::type)lhs) & ((std::underlying_type<T>::type)rhs));		\
		}																								\
																										\
		inline T& operator&=(T& lhs, T rhs) noexcept													\
		{																								\
			return (T&)(((std::underlying_type<T>::type&)lhs) &= ((std::underlying_type<T>::type)rhs));	\
		}																								\
																										\
		constexpr inline T operator~(T lhs) noexcept													\
		{																								\
			return T(~((std::underlying_type<T>::type)lhs));											\
		}																								\
																										\
		constexpr inline T operator^(T lhs, T rhs) noexcept												\
		{																								\
			return T(((std::underlying_type<T>::type)lhs) ^ ((std::underlying_type<T>::type)rhs));		\
		}																								\
																										\
		inline T& operator^=(T& lhs, T rhs) noexcept													\
		{																								\
			return (T&)(((std::underlying_type<T>::type&)lhs) ^= ((std::underlying_type<T>::type)rhs));	\
		}																								\
																										\
		constexpr inline bool operator==(T lhs, std::int32_t rhs) noexcept								\
		{																								\
			return ((std::underlying_type<T>::type)lhs) == ((std::underlying_type<T>::type)rhs);		\
		}																								\
																										\
		constexpr inline bool operator!=(T lhs, std::int32_t rhs) noexcept								\
		{																								\
			return ((std::underlying_type<T>::type)lhs) != ((std::underlying_type<T>::type)rhs);		\
		}																								\
	}
