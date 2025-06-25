cmake_minimum_required(VERSION 3.15)
project(csvorm)

add_library(csvorm INTERFACE)

target_include_directories(csvorm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/csvorm.hpp)

target_compile_features(csvorm INTERFACE cxx_std_17)