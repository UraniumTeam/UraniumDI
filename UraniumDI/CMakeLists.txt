set(SRC
    UnDI/Base/Base.h
    UnDI/Base/CompilerTraits.h
    UnDI/Base/Platform.h
    UnDI/Base/PlatformTraits.h

    UnDI/Memory/IAllocator.h
    UnDI/Memory/Memory.h
    UnDI/Memory/Object.h
    UnDI/Memory/Ptr.h
    UnDI/Memory/ReferenceCounter.h
    UnDI/Memory/SystemAllocator.h

    UnDI/RTTI/RTTI.h

    UnDI/Utils/UUID.h
    UnDI/Utils/Result.h
    UnDI/Utils/ErrorCode.h

    UnDI/Container.h
    UnDI/Container.cpp
)

add_library(UnDI STATIC ${SRC})

un_configure_target(UnDI)

target_include_directories(UnDI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(UnDI PROPERTIES FOLDER "UraniumDI")

get_property("TARGET_SOURCE_FILES" TARGET UnDI PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

add_subdirectory(Tests)
