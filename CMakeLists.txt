cmake_minimum_required(VERSION 3.28)

project(chenchen)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_subdirectory(3rd_party)
add_subdirectory(src)