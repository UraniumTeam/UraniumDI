#pragma once
#include <csignal>
#include <malloc.h>

#if defined _WIN32 || defined _WIN64 || defined _WINDOWS
#    define UN_WINDOWS 1

#    define UN_DLL_EXTENSION ".dll"
#    define UN_EXE_EXTENSION ".exe"
#    define UN_PATH_SEPARATOR '\\'
#    define UN_OS_NAME "Windows"

#    define UN_ALIGNED_MALLOC(size, alignment) _aligned_malloc(size, alignment)
#    define UN_ALIGNED_FREE(ptr) _aligned_free(ptr)

#    define UN_BYTE_SWAP_UINT16(value) _byteswap_ushort(value)
#    define UN_BYTE_SWAP_UINT32(value) _byteswap_ulong(value)
#    define UN_BYTE_SWAP_UINT64(value) _byteswap_uint64(value)

#    define UN_DLL_EXPORT __declspec(dllexport)
#    define UN_DLL_IMPORT __declspec(dllimport)

#elif defined __linux__
#    define UN_LINUX 1

#    define UN_DLL_EXTENSION ".so"
#    define UN_EXE_EXTENSION ""
#    define UN_PATH_SEPARATOR '/'
#    define UN_OS_NAME "Linux"

#    define UN_ALIGNED_MALLOC(size, alignment) ::memalign(alignment, size)
#    define UN_ALIGNED_FREE(ptr) ::free(ptr)

#    define UN_BYTE_SWAP_UINT16(value) __builtin_bswap16(value)
#    define UN_BYTE_SWAP_UINT32(value) __builtin_bswap32(value)
#    define UN_BYTE_SWAP_UINT64(value) __builtin_bswap64(value)

#    define UN_DLL_EXPORT __attribute__((visibility("default")))
#    define UN_DLL_IMPORT __attribute__((visibility("default")))

#else
#    error Unsupported platform
#endif
