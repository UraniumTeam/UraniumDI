add_executable(ConfigurationSample main.cpp)

un_configure_target(ConfigurationSample)

set_target_properties(ConfigurationSample PROPERTIES FOLDER "Samples")
target_link_libraries(ConfigurationSample UnDI)
