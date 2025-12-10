cmake_minimum_required(VERSION 3.15)
project(csvorm)

# Header-only library
add_library(csvorm INTERFACE)

# Correct include directory: enables #include <csvorm>
target_include_directories(csvorm INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Require C++17 for consumers
target_compile_features(csvorm INTERFACE cxx_std_17)
