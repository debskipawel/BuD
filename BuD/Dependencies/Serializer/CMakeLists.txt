cmake_minimum_required(VERSION 3.10)

project(MGSerializer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

include_directories(dependencies/valijson/include)
include_directories(dependencies/json/include)

# Aktualnie zależności między Serializerem i Sanboxem są dziwne - jedno zawsze kompiluje drugie
# Zamysł był takie, aby Serializer kompilował tylko bibliotekę
# Jak ktos będzie miec lepszy pomysł jak poprawic zależności pomiędzy Sandboxem a Serializerem to prosze bardzo
add_subdirectory(Serializer)
add_subdirectory(Sandbox)
