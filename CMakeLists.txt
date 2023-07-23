cmake_minimum_required(VERSION 3.24)
project(UraniumDI VERSION 0.1.0
    LANGUAGES CXX C
)

enable_testing()
set(UN_PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
list(PREPEND CMAKE_MODULE_PATH "${UN_PROJECT_ROOT}/cmake")

include(Config)
include(ThirdParty/gtest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(UraniumDI)
add_subdirectory(Samples)
