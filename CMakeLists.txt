cmake_minimum_required(VERSION 3.5)
project(simple-rsa-cracker)

set(CMAKE_CXX_STANDARD 11)

add_executable(RSA_Cracker_BoostMpi rsaCrackerBoostMpi.cpp)
add_executable(RSA_Cracker_OpenMp rsaCrackerOpenMp.cpp)

find_package(Boost REQUIRED mpi serialization)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(RSA_Cracker_BoostMpi ${Boost_LIBRARIES})

find_package(MPI REQUIRED)
include_directories(${MPI_CXX_INCLUDE_PATH})
target_link_libraries(RSA_Cracker_BoostMpi ${MPI_CXX_LIBRARIES})

find_package(OpenMP)
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()