cmake_minimum_required(VERSION 3.21)

project(find-peaks)

set(SOURCES
    PeakFinder.h
    PeakFinder.cpp
)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable (example example.cpp ${SOURCES})
