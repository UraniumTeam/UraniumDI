if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(UN_COMPILER_CLANG ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(UN_COMPILER_GCC ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(UN_COMPILER_MSVC ON)
endif ()

if (WIN32)
    set(UN_WINDOWS ON)
endif()

set(CMAKE_DEBUG_POSTFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

if (UN_COMPILER_MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
    add_compile_definitions(_ENABLE_EXTENDED_ALIGNED_STORAGE)
    set(CMAKE_USE_WIN32_THREADS_INIT ON)
endif ()

function(un_configure_target TARGET)
    if (UN_COMPILER_MSVC)
        target_compile_options(${TARGET} PRIVATE /W4 /WX)
    else ()
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Werror -fPIC)
    endif ()
endfunction()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
