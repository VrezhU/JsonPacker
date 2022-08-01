cmake_minimum_required(VERSION 3.5)

project(JsonTLV LANGUAGES CXX VERSION 0.0.1)
message("Project: ${PROJECT_NAME}:${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

include_directories(headers)
add_library(${PROJECT_NAME} JsonTLV.cpp JsonTLV.h)
