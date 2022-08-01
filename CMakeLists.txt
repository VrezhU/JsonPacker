cmake_minimum_required(VERSION 3.5)

project(JsonPacker LANGUAGES CXX VERSION 0.0.1)
message("Project: ${PROJECT_NAME}:${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/JsonTLV)
add_subdirectory(${CMAKE_SOURCE_DIR}/JsonTLV)
add_executable(${PROJECT_NAME} main.cpp)

#
include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

#ExternalProject_Add(nlohmann
#    GIT_REPOSITORY https://github.com/nlohmann/json.git
#    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
#)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
#add_dependencies(${PROJECT_NAME} nlohmann)

#

target_link_libraries(${PROJECT_NAME}
#    nlohmann
    JsonTLV
)
