cmake_minimum_required(VERSION 3.5)

project(FlowLink VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_STATIC "Build the static library" OFF)

add_subdirectory(3rdparty/ads)
add_subdirectory(3rdparty/plog)
add_subdirectory(desktop)
