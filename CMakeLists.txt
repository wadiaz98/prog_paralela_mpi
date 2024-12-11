cmake_minimum_required(VERSION 3.29)
project(mpi)

set(CMAKE_CXX_STANDARD 17)

find_package(MPI)

if(MPI_FOUND)
    message(STATUS "MPI FOUND")
    message(STATUS "MPI Version: ${MPI_CXX_VERSION}")
    message(STATUS "MPI Include Path: ${MPI_INCLUDE_PATH}")
    message(STATUS "MPI Libraries: ${MPI_LIBRARIES}")
    message(STATUS "MPI Compiler: ${MPI_CXX_COMPILER}")
    message(STATUS "MPI Link Flags: ${MPI_LINK_FLAGS}")
else()
    message(FATAL_ERROR "MPI NOT FOUND")
endif ()

include_directories($MPI_INCLUDE_PATH)

add_executable(mpi01 main.cpp)
target_link_libraries(mpi01 ${MPI_LIBRARIES})
