set(SRC
    UnDI/Builder/ContainerBuilder.h
    UnDI/Builder/ContainerBuilder.cpp

    UnDI/Lifetime/ILifetimeScope.h
    UnDI/Lifetime/LifetimeScope.h
    UnDI/Lifetime/LifetimeScope.cpp

    UnDI/Registry/IServiceRegistry.h
    UnDI/Registry/ServiceRegistry.h
    UnDI/Registry/ServiceRegistry.cpp
    UnDI/Registry/ServiceRegistryBuilder.h
    UnDI/Registry/ServiceRegistryBuilder.cpp

    UnDI/Utils/ErrorCode.h

    UnDI/IContainer.h
    UnDI/Container.h
    UnDI/Container.cpp
    UnDI/ServiceRegistration.h
    UnDI/ServiceActivator.h
    UnDI/IServiceProvider.h
    UnDI/Utils/ErrorCode.h
)

add_library(UnDI STATIC ${SRC})

un_configure_target(UnDI)

target_include_directories(UnDI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(UnDI PROPERTIES FOLDER "UraniumDI")
target_link_libraries(UnDI UnTL)

get_property("TARGET_SOURCE_FILES" TARGET UnDI PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

add_subdirectory(Tests)
