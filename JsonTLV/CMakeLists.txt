cmake_minimum_required(VERSION 3.5)

project(JsonTLV LANGUAGES CXX VERSION 0.0.1)
message("Project: ${PROJECT_NAME}:${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(headers)
add_library(${PROJECT_NAME} JsonTLV.cpp JsonTLV.h)
