cmake_minimum_required(VERSION 3.25)

project(mtd CXX)

set(SOURCES
	source/game.cpp)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -O2 -Wextra -Wnon-virtual-dtor -pedantic -Werror)

include_directories(
	./include)

add_library(mtd OBJECT ${SOURCES})

add_executable(main
	source/main.cpp
	$<TARGET_OBJECTS:mtd>)
