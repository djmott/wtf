cmake_minimum_required(VERSION 2.6)

project(wtf)

find_package(Doxygen)

option(WTF_DEBUG_MESSAGES "Debug windows messages" FALSE)
mark_as_advanced(WTF_DEBUG_MESSAGES)
if(WTF_DEBUG_MESSAGES)
	add_definitions(-D__WTF_DEBUG_MESSAGES__=1)
endif()

file(GLOB_RECURSE HEADERS include/*.hpp)

set(WTF_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(wtf SOURCES ${HEADERS})

include_directories(include)


add_subdirectory(examples)


if(DOXYGEN_FOUND)
  option(BUILD_DOCS "Build doxygen documentation" NO)
else()
  set(BUILD_DOCS NO)
endif()

if(BUILD_DOCS AND DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
  add_custom_target(
    docs COMMAND "${DOXYGEN_EXECUTABLE}" Doxyfile
    COMMENT "Generating Doxygen API Documentation" VERBATIM
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs"
  )
endif()
