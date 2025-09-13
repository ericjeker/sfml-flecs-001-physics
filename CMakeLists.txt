cmake_minimum_required(VERSION 3.31)
project(SFMLFlecsPhysics LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

message("--- Adding Include")
add_subdirectory(include)
add_subdirectory(vendor)
add_subdirectory(src)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
