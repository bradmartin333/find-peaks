cmake_minimum_required(VERSION 3.21)

project(find-peaks)

include_directories(.)

add_executable (example example.cpp PeakFinder.cpp)
target_link_libraries (example)

