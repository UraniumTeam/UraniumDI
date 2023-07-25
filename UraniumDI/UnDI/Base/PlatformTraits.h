#pragma once
#include <csignal>
#include <malloc.h>

#if defined _WIN32 || defined _WIN64 || defined _WINDOWS
#    define UN_WINDOWS 1

#    define UN_DLL_EXTENSION ".dll"
#    define UN_EXE_EXTENSION ".exe"
#    define UN_PATH_SEPARATOR '\\'
#    define UN_OS_NAME "Windows"

#    define UN_AlignedMalloc(size, alignment) _aligned_malloc(size, alignment)
#    define UN_AlignedFree(ptr) _aligned_free(ptr)

#    define UN_ByteSwapUInt16(value) _byteswap_ushort(value)
#    define UN_ByteSwapUInt32(value) _byteswap_ulong(value)
#    define UN_ByteSwapUInt64(value) _byteswap_uint64(value)

#    define UN_DLL_EXPORT __declspec(dllexport)
#    define UN_DLL_IMPORT __declspec(dllimport)

#elif defined __linux__
#    define UN_LINUX 1

#    define UN_DLL_EXTENSION ".so"
#    define UN_EXE_EXTENSION ""
#    define UN_PATH_SEPARATOR '/'
#    define UN_OS_NAME "Linux"

#    define UN_AlignedMalloc(size, alignment) ::memalign(alignment, size)
#    define UN_AlignedFree(ptr) ::free(ptr)

#    define UN_ByteSwapUInt16(value) __builtin_bswap16(value)
#    define UN_ByteSwapUInt32(value) __builtin_bswap32(value)
#    define UN_ByteSwapUInt64(value) __builtin_bswap64(value)

#    define UN_DLL_EXPORT __attribute__((visibility("default")))
#    define UN_DLL_IMPORT __attribute__((visibility("default")))

#else
#    error Unsupported platform
#endif
