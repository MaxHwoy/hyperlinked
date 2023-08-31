#pragma once

#ifdef HYPER_VALID_PLATFORM
#undef HYPER_VALID_PLATFORM
#endif

#ifdef PLATFORM_CYGWIN
#undef PLATFORM_CYGWIN
#endif

#ifdef PLATFORM_MINGW
#undef PLATFORM_MINGW
#endif

#ifdef PLATFORM_DESKTOP
#undef PLATFORM_DESKTOP
#endif

#ifdef PLATFORM_CONSOLE
#undef PLATFORM_CONSOLE
#endif

#ifdef PLATFORM_PS4
#undef PLATFORM_PS4
#endif

#ifdef PLATFORM_XBOXONE
#undef PLATFORM_XBOXONE
#endif

#ifdef PLATFORM_LINUX
#undef PLATFORM_LINUX
#endif

#ifdef PLATFORM_OSX
#undef PLATFORM_OSX
#endif

#ifdef PLATFORM_WINDOWS
#undef PLATFORM_WINDOWS
#endif

#ifdef PLATFORM_ANDROID
#undef PLATFORM_ANDROID
#endif

#ifdef PLATFORM_IPHONE
#undef PLATFORM_IPHONE
#endif

#ifdef PLATFORM_LRB
#undef PLATFORM_LRB
#endif

#ifdef PLATFORM_BSD
#undef PLATFORM_BSD
#endif

#ifdef PLATFORM_NAME
#undef PLATFORM_NAME
#endif

#ifdef PLATFORM_DESCRIPTION
#undef PLATFORM_DESCRIPTION
#endif

#ifdef TARGET_64BIT
#undef TARGET_64BIT
#endif

#ifdef MISALIGNED_SUPPORT_LEVEL
#undef MISALIGNED_SUPPORT_LEVEL
#endif

#ifdef CACHE_LINE_SIZE
#undef CACHE_LINE_SIZE
#endif



// PLAYSTATION 4
#if defined(__ORBIS__)
#define HYPER_VALID_PLATFORM 1
#define PLATFORM_PS4
#define PLATFORM_NAME "PS4"
#define PLATFORM_DESCRIPTION "PS4 on x64"
#define PROCESSOR_X86_64
#define PLATFORM_CONSOLE
#endif

// XBOXONE
#if defined(_DURANGO) || defined(_XBOX_ONE) || defined(_GAMING_XBOX)
#define HYPER_VALID_PLATFORM 1
#define PLATFORM_XBOXONE
#define PLATFORM_NAME "XBox One"
#define PLATFORM_DESCRIPTION "XBox One on x64"
#define PROCESSOR_X86_64
#define PLATFORM_CONSOLE
#endif

// LRB
#if defined(__LRB__) || (defined(__EDG__) && defined(__ICC) && defined(__x86_64__))
#define HYPER_VALID_PLATFORM 1
#define PLATFORM_LRB
#define PLATFORM_NAME "Larrabee"
#define PLATFORM_DESCRIPTION "Larrabee on LRB1"
#define PROCESSOR_X86_64
#define PLATFORM_DESKTOP
#if defined(BYTE_ORDER) && (BYTE_ORDER == 4321)
#define SYSTEM_BIG_ENDIAN
#endif
#endif

// ANDROID
#if defined(__ANDROID__)
#define HYPER_VALID_PLATFORM 1
#define PLATFORM_ANDROID
#define PLATFORM_NAME "Android"
#if defined(__arm__)
#define PLATFORM_DESCRIPTION "Android on ARM"
#define PROCESSOR_ARM32
#elif defined(__aarch64__)
#define PLATFORM_DESCRIPTION "Android on ARM64"
#define PROCESSOR_ARM64
#elif defined(__i386__)
#define PLATFORM_DESCRIPTION "Android on x86"
#define PROCESSOR_X86
#elif defined(__x86_64)
#define PLATFORM_DESCRIPTION "Android on x64"
#define PROCESSOR_X86_64
#else
#error Unknown processor
#endif
#define PLATFORM_MOBILE
#endif

// APPLE
#if defined(__APPLE__) && __APPLE__
#include <TargetConditionals.h>

// IPHONE
#if defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
#define HYPER_VALID_PLATFORM 1
#define PLATFORM_IPHONE
#define PLATFORM_NAME "iPhone"
#if defined(__arm__)
#define PLATFORM_DESCRIPTION "iPhone on ARM"
#define PROCESSOR_ARM32
#elif defined(__aarch64__) || defined(__AARCH64)
#define PLATFORM_DESCRIPTION "iPhone on ARM64"
#define PROCESSOR_ARM64
#elif defined(__i386__)
#define PLATFORM_DESCRIPTION "iPhone simulator on x86"
#define PROCESSOR_X86
#elif defined(__x86_64) || defined(__amd64)
#define PLATFORM_DESCRIPTION "iPhone simulator on x64"
#define PROCESSOR_X86_64
#else
#error Unknown processor
#endif
#define PLATFORM_MOBILE
#endif

// OSX
#if defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
#define HYPER_VALID_PLATFORM 1
#define PLATFORM_OSX
#define PLATFORM_NAME "OSX"
#if defined(__i386__) || defined(__intel__)
#define PLATFORM_DESCRIPTION "OSX on x86"
#define PROCESSOR_X86
#elif defined(__x86_64) || defined(__amd64)
#define PLATFORM_DESCRIPTION "OSX on x64"
#define PROCESSOR_X86_64
#elif defined(__arm__)
#define PLATFORM_DESCRIPTION "OSX on ARM"
#define PROCESSOR_ARM32
#elif defined(__aarch64__) || defined(__AARCH64)
#define PLATFORM_DESCRIPTION "OSX on ARM64"
#define PROCESSOR_ARM64
#elif defined(__POWERPC64__) || defined(__powerpc64__)
#define SYSTEM_BIG_ENDIAN 1
#define PLATFORM_DESCRIPTION "OSX on PowerPC 64"
#define PROCESSOR_POWERPC_64
#elif defined(__POWERPC__) || defined(__powerpc__)
#define SYSTEM_BIG_ENDIAN 1
#define PLATFORM_DESCRIPTION "OSX on PowerPC"
#define PROCESSOR_POWERPC_32
#else
#error Unknown processor
#error Unknown endianness
#endif
#define PLATFORM_DESKTOP
#endif

#endif

// LINUX
#if defined(__linux) || defined(__linux__)
#define HYPER_VALID_PLATFORM 1
#define PLATFORM_LINUX
#define PLATFORM_NAME "Linux"
#if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
#define PLATFORM_DESCRIPTION "Linux on x86"
#define PROCESSOR_X86
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_EABI__)
#define PLATFORM_DESCRIPTION "Linux on ARM 6/7 32-bits"
#define PROCESSOR_ARM32
#elif defined(__aarch64__) || defined(__AARCH64)
#define PLATFORM_DESCRIPTION "Linux on ARM64"
#define PROCESSOR_ARM64
#elif defined(__x86_64__)
#define PLATFORM_DESCRIPTION "Linux on x64"
#define PROCESSOR_X86_64
#elif defined(__powerpc64__)
#define SYSTEM_BIG_ENDIAN 1
#define PLATFORM_DESCRIPTION "Linux on PowerPC 64"
#define PROCESSOR_POWERPC_64
#elif defined(__powerpc__)
#define SYSTEM_BIG_ENDIAN 1
#define PLATFORM_DESCRIPTION "Linux on PowerPC"
#define PROCESSOR_POWERPC_32
#else
#error Unknown processor
#error Unknown endianness
#endif
#define PLATFORM_DESKTOP
#endif

// BSD
#if defined(__BSD__) || defined(__FreeBSD__)
#define HYPER_VALID_PLATFORM 1
#define PLATFORM_BSD
#define PLATFORM_NAME "BSD Unix"
#if defined(__i386__) || defined(__intel__)
#define PLATFORM_DESCRIPTION "BSD on x86"
#define PROCESSOR_X86
#elif defined(__x86_64__)
#define PLATFORM_DESCRIPTION "BSD on x64"
#define PROCESSOR_X86_64
#elif defined(__powerpc64__)
#define SYSTEM_BIG_ENDIAN 1
#define PLATFORM_DESCRIPTION "BSD on PowerPC 64"
#define PROCESSOR_POWERPC_64
#elif defined(__powerpc__)
#define SYSTEM_BIG_ENDIAN 1
#define PLATFORM_DESCRIPTION "BSD on PowerPC"
#define PROCESSOR_POWERPC_32
#else
#error Unknown processor
#error Unknown endianness
#endif
#define PLATFORM_DESKTOP
#endif

// WINDOWS
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#define HYPER_VALID_PLATFORM 1
#define PLATFORM_WINDOWS
#define PLATFORM_NAME "Windows"
#if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
#define PLATFORM_DESCRIPTION "Windows on x64"
#define PROCESSOR_X86_64
#elif defined(_M_IX86) || defined(_X86_)
#define PLATFORM_DESCRIPTION "Windows on X86"
#define PROCESSOR_X86
#elif defined(_M_IA64) || defined(_IA64_)
#define PLATFORM_DESCRIPTION "Windows on IA-64"
#define PROCESSOR_IA64
#elif defined(_M_ARM)
#define PLATFORM_DESCRIPTION "Windows on ARM"
#define PROCESSOR_ARM32
#elif defined(_M_ARM64)
#define PLATFORM_DESCRIPTION "Windows on ARM64"
#define PROCESSOR_ARM64
#else
#error Unknown processor
#error Unknown endianness
#endif
#define PLATFORM_DESKTOP
#endif


#ifndef HYPER_VALID_PLATFORM
#error "Unknown platform"
#endif



// TARGET_64BIT
// Indicates whether a targeted platform is 64-bit (sizeof(void*) == 8)
#if   defined(_WIN64) || defined(__LP64__) || defined(_LP64) || defined(_M_IA64) || defined(__ia64__) || defined(__arch64__) || defined(__aarch64__) || defined(__mips64__) || defined(__64BIT__) || defined(__Ptr_Is_64)
#define TARGET_64BIT
#elif defined(__CC_ARM) && (__sizeof_ptr == 8)
#define TARGET_64BIT
#endif



// MISALIGNED_SUPPORT_LEVEL
// Specifies if the processor can read and write built-in types that aren't
// naturally aligned.
//    0 - not supported. Likely causes an exception.
//    1 - supported but slow.
//    2 - supported and fast.
#if defined(PROCESSOR_X86_64)
#define MISALIGNED_SUPPORT_LEVEL 2
#else
#define MISALIGNED_SUPPORT_LEVEL 0
#endif



// CACHE_LINE_SIZE
// Specifies the cache line size broken down by compile target.
// This the expected best guess values for the targets that we can make at compilation time.
#if   defined(PROCESSOR_X86)      
#define CACHE_LINE_SIZE 32
#elif defined(PROCESSOR_X86_64)  
#define CACHE_LINE_SIZE 64
#elif defined(PROCESSOR_ARM32)
#define CACHE_LINE_SIZE 32
#elif defined(PROCESSOR_ARM64)
#define CACHE_LINE_SIZE 64
#elif defined(TARGET_64BIT)
#define CACHE_LINE_SIZE 64
#else
#define CACHE_LINE_SIZE 32
#endif

// PRODUCT_NAME
#ifdef PRODUCT_NAME
#undef PRODUCT_NAME
#endif

#define PRODUCT_NAME            "Hyperlinked"
#define PRODUCT_NAME_LOWER_CASE "hyperlinked"
