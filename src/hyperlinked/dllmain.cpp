#include <Windows.h>
#include <cstdio>
#include <iostream>

#include <hyperlinked/patches.hpp>

#pragma warning (disable : 6031)

#define RUN_TESTS

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
            ::AllocConsole();

            ::freopen("CONIN$", "r", stdin);
            ::freopen("CONOUT$", "w", stdout);
            ::freopen("CONOUT$", "w", stderr);

            int temp;

            std::cin >> temp;

            hyper::patches::init();

#if defined(RUN_TESTS)
            hyper::tests::init();
#endif
        }
        else
        {
            MessageBoxA(NULL, "This .exe is not supported!", "hyperlink", MB_ICONERROR);

            return FALSE;
        }
    }

    return TRUE;
}
