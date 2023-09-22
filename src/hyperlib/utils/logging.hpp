#pragma once

#include <cstdio>
#include <mutex>
#include <hyperlib/platform.hpp>

#if defined(PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace hyper
{
	class logging final
	{
	public:
		enum class ptr_type
		{
			console_out,
			console_in,
			file_out,
			count,
		};

	public:
		static void init(const char* path, bool allocate_console, bool pause);
		static void free();

		static void log(const char* message, ...);

	private:
		static inline std::mutex mutex_;

		static inline ::FILE* pointers_[static_cast<size_t>(ptr_type::count)];
		static inline char buffer_[1024 + 1];

#if defined(PLATFORM_WINDOWS)
		static inline ::HANDLE handle_;
		static inline ::DWORD mode_;
#endif
	};
}

#if defined(PLATFORM_WINDOWS)
#define PRINT_ENVIRONMENT_NEW_LINE "\r\n"
#else
#define PRINT_ENVIRONMENT_NEW_LINE "\n"
#endif

#define PRINT_IMPLEMENTATION(COLOR, CHANNEL, MESSAGE, ...)																		\
	::hyper::logging::log("[" "\x1B[" COLOR "m" CHANNEL "\033[0m" "] " MESSAGE PRINT_ENVIRONMENT_NEW_LINE, ##__VA_ARGS__)

#if defined(_DEBUG)
#define PRINT_TRACE(MESSAGE, ...) PRINT_IMPLEMENTATION("39", "  TRACE", MESSAGE, ##__VA_ARGS__)
#else
#define PRINT_TRACE(...)
#endif

#if !defined(BUILD_PRODUCTION)
#define PRINT_DEBUG(MESSAGE, ...) PRINT_IMPLEMENTATION("96", "  DEBUG", MESSAGE, ##__VA_ARGS__)
#else
#define PRINT_DEBUG(...)
#endif

#define PRINT_INFO(MESSAGE, ...)    PRINT_IMPLEMENTATION("97", "   INFO", MESSAGE, ##__VA_ARGS__)
#define PRINT_WARNING(MESSAGE, ...) PRINT_IMPLEMENTATION("93", "WARNING", MESSAGE, ##__VA_ARGS__)
#define PRINT_FATAL(MESSAGE, ...)   PRINT_IMPLEMENTATION("91", "  FATAL", MESSAGE, ##__VA_ARGS__)
