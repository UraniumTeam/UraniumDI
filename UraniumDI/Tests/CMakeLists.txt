set(SRC
    Common/Common.h

    Test.cpp
    main.cpp
)

add_executable(UnDITests ${SRC})

set_target_properties(UnDITests PROPERTIES FOLDER "UraniumDI")
target_link_libraries(UnDITests gtest gmock UnDI)

get_property("TARGET_SOURCE_FILES" TARGET UnDITests PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})

gtest_discover_tests(UnDITests)
