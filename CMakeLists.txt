cmake_minimum_required(VERSION 2.6)
project(wtf)

file(GLOB headers *.hpp)
file(GLOB_RECURSE themes themes/*.hpp)

add_executable(wtf wtf.cpp ${headers} ${themes})

install(TARGETS wtf RUNTIME DESTINATION bin)
