#include <Windows.h>
#include <cstdio>
#include <iostream>

#include <hyperlib/utils/logging.hpp>
#include <hyperlib/hook.hpp>
#include <hyperlinked/patches.hpp>

#pragma warning (disable : 6031)

#undef CONSOLEON
#undef RUN_TESTS

#if defined(RUN_TESTS)
#include <hyperlinked/tests.hpp>
#endif

constexpr auto MAIN_ENTRY_POINT = 0x0087E926;

BOOL APIENTRY DllMain(HMODULE, DWORD ul_reason_for_call, LPVOID)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        auto base = (uintptr_t)::GetModuleHandleA(NULL);
        auto dos = (IMAGE_DOS_HEADER*)(base);
        auto nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
        auto entry = base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base);

        if (entry == MAIN_ENTRY_POINT)
        {
#if defined(RUN_TESTS) && defined(CONSOLEON)
            hyper::logging::init(nullptr, true, true);
#endif
            hyper::patches::init();
#if defined(RUN_TESTS)
            hyper::tests::init();
#endif
        }
        else
        {
            ::MessageBoxA(NULL, "This .exe is not supported!", PRODUCT_NAME, MB_ICONERROR);

            return FALSE;
        }
    }

    return TRUE;
}
