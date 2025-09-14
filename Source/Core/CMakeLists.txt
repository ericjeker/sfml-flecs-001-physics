file(GLOB_RECURSE SOURCES Private/*.cpp)
add_library(Core)
target_sources(Core PRIVATE ${SOURCES})
target_include_directories(Core PUBLIC Public)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif ()
