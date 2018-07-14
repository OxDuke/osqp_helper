cmake_minimum_required( VERSION 2.8 )

## Required software
find_package( osqp REQUIRED )

## Sources and headers
include_directories( ${osqp_INCLUDE_DIRS} )

message (STATUS ${osqp_INCLUDE_DIRS})

## Find all source files to be compiled into object files
#file( GLOB SOURCES *.cc *.cpp )

## C++ compiler options
set( CMAKE_CXX_FLAGS "-Wall -Wextra" )
set( CMAKE_CXX_FLAGS_DEBUG "-g -O0" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

## Source file containing the "main" function
#set( MAIN_SOURCES main.cpp )
## Specify a name for the generated executable file
#set( MAIN_EXE_NAME generic_executable )

add_executable( demo demo.cpp osqp_helper.cpp)

target_link_libraries( demo  PRIVATE osqp::osqp)
#target_link_libraries( demo  osqp)



