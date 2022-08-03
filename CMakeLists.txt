cmake_minimum_required(VERSION 3.5)

project(JsonPacker LANGUAGES CXX VERSION 0.0.1)
message("Project: ${PROJECT_NAME}:${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(${CMAKE_SOURCE_DIR}/JsonTLV)
include_directories(${CMAKE_SOURCE_DIR}/JsonTLV)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    JsonTLV
)
