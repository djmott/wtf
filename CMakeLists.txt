cmake_minimum_required(VERSION 2.6)

project(wtf)

file(GLOB_RECURSE HEADERS include/*.hpp)

add_custom_target(wtf SOURCES ${HEADERS})

include_directories(include)


add_subdirectory(examples)

