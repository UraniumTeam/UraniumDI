add_executable(SampleProject main.cpp)

un_configure_target(SampleProject)

set_target_properties(SampleProject PROPERTIES FOLDER "Samples")
target_link_libraries(SampleProject UnDI)
