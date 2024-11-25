cmake_minimum_required(VERSION 3.25)

project(mtd CXX)

set(SOURCES
	source/game.cpp)

set(TESTS test_shuffle)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -O2 -Wextra -Wnon-virtual-dtor -pedantic -Werror)

include_directories(
	./include)

foreach(t ${TESTS})
  add_executable(${t} test/${t}.cpp $<TARGET_OBJECTS:mtd>)
  add_test(NAME ${t} COMMAND ${t} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()
enable_testing()

add_library(mtd OBJECT ${SOURCES})

add_executable(main
	source/main.cpp
	$<TARGET_OBJECTS:mtd>)
