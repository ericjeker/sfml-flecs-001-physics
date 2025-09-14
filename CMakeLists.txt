cmake_minimum_required(VERSION 3.31)
project(SFMLFlecsPhysics LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

message("--- Adding Vendor")
add_subdirectory(Vendor)

message("--- Adding Source")
add_subdirectory(Source)
