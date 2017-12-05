project(LP_MP-MRF)
cmake_minimum_required(VERSION 2.8.12)

# C++11
add_compile_options(-std=c++17)

# compiler options
add_definitions(-march=native)

include_directories(include)
include_directories(external)
add_subdirectory(external/LP_MP)
include_directories(external/arboricity)
add_subdirectory(external/arboricity)
include_directories(external/PEGTL)
include_directories(external/opengm/include)

# for opengm
find_package(HDF5 1.8.15 REQUIRED)
include_directories (${HDF5_INCLUDE_DIR})
add_definitions(${HDF5_DEFINITIONS})
message(STATUS ${HDF5_LIBRARIES})
message(STATUS ${HDF5_INCLUDE_DIR})

add_subdirectory(test)
add_subdirectory(src)
