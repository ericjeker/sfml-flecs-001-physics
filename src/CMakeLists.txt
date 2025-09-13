message("--- Finding Sources")
file(GLOB_RECURSE SOURCES *.cpp)

message("--- Adding Executable")
add_executable(main)
target_sources(main PRIVATE ${SOURCES})
target_link_libraries(main PRIVATE include flecs SFML::Window SFML::Graphics)

