cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FILES_DIRECTORY "${CMAKE_HOME_DIRECTORY}/cmake")

include(${CMAKE_FILES_DIRECTORY}/compiler.cmake)
include(${CMAKE_FILES_DIRECTORY}/output.cmake)

project(nk-engine)

add_subdirectory(engine)
add_subdirectory(editor)
