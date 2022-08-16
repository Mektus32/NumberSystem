cmake_minimum_required(VERSION 3.23)
project(NumberSystem)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package( Boost 1.40 COMPONENTS program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

add_executable(NumberSystem main.cpp)


target_link_libraries( NumberSystem LINK_PUBLIC ${Boost_LIBRARIES} )