#include <hyperlib/utils/logging.hpp>
#include <cstdarg>
#include <cassert>

namespace hyper
{
	void logging::init(const char* path, bool allocate_console)
	{
		const auto _ = std::lock_guard(logging::mutex_);

		if (allocate_console)
		{
#if defined(PLATFORM_WINDOWS)
			::AllocConsole();
#endif
			errno_t result = ::freopen_s(&logging::pointers_[0], "CONOUT$", "w", stdout);

			assert(!result);

#if defined(PLATFORM_WINDOWS)
			::SetConsoleTitleA("Need for Speed: Carbon (Debug Console)");
			::SetConsoleOutputCP(CP_UTF8);

			const auto handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

			logging::handle_ = handle;

			::GetConsoleMode(handle, &logging::mode_);
			::SetConsoleMode(handle, (logging::mode_ | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN) & ~ENABLE_QUICK_EDIT_MODE);
#endif
		}

		if (path != nullptr)
		{
			errno_t result = ::fopen_s(&logging::pointers_[1], path, "ab");

			assert(!result);
		}
	}

	void logging::free()
	{
		const auto _ = std::lock_guard(logging::mutex_);
#if defined(PLATFORM_WINDOWS)
		if (logging::pointers_[0] != nullptr)
		{
			::SetConsoleMode(logging::handle_, logging::mode_);
		}
#endif
		for (auto& pointer : logging::pointers_)
		{
			if (pointer != nullptr)
			{
				::fclose(pointer);

				pointer = nullptr;
			}
		}
	}

	void logging::log(const char* message, ...)
	{
		const auto _ = std::lock_guard(logging::mutex_);

		if (logging::pointers_[0] == nullptr && logging::pointers_[1] == nullptr)
		{
			return;
		}

		::va_list va = {};

		va_start(va, message);

		auto& buffer = logging::buffer_;

		::vsnprintf(buffer, std::size(buffer), message, va);

		va_end(va);

		// skip color codes and channel names when printing elsewhere.
		const auto offset = ::strstr(buffer, "] ") + 2;
#if defined(PLATFORM_WINDOWS)
		::OutputDebugStringA(offset);
#endif
		const auto format = "%s";

		if (logging::pointers_[0] != nullptr)
		{
			::printf(format, buffer);
		}

		if (const auto pointer = logging::pointers_[1])
		{
			::fprintf(pointer, format, offset);
			::fflush(pointer);
		}
	}
}
