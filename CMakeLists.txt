cmake_minimum_required(VERSION 3.10) 
project(snake)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(snake main.cpp render.cpp snake.cpp)
target_link_libraries(snake ${CURSES_LIBRARIES})
target_include_directories(snake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})