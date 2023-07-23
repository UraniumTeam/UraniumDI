set(SRC
    main.cpp
)

add_executable(UnComputeTests ${SRC})

set_target_properties(UnComputeTests PROPERTIES FOLDER "UraniumCompute")
target_link_libraries(UnComputeTests gtest gmock UnCompute)

get_property("TARGET_SOURCE_FILES" TARGET UnComputeTests PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

gtest_discover_tests(UnComputeTests)
