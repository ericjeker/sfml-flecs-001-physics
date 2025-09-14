message("--- Finding Sources")
file(GLOB_RECURSE SOURCES Private/*.cpp)

message("--- Adding Executable")
add_executable(GamePhysicsEngine)
target_sources(GamePhysicsEngine PRIVATE ${SOURCES})
target_include_directories(GamePhysicsEngine PUBLIC Public)
target_link_libraries(GamePhysicsEngine PRIVATE Core PhysicsModule flecs SFML::Window SFML::Graphics)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif ()
