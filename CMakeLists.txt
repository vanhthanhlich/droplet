cmake_minimum_required(VERSION 3.10)

project(droplet)

set(CMAKE_CXX_STANDARD 23)

find_package(Curses REQUIRED)
include_directories(include)
include_directories(${CURSES_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS context)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cpp")



add_executable(main ${SOURCES})

target_link_libraries(main ${CURSES_LIBRARIES})
target_link_libraries(main ${Boost_LIBRARIES})