project(LP_MP-MRF)
cmake_minimum_required(VERSION 2.8.12)

# C++11
add_compile_options(-std=c++14)

# compiler options
add_definitions(-march=native)

include_directories(include)
include_directories(external)
add_subdirectory(external/LP_MP)
include_directories(external/arboricity)
add_subdirectory(external/arboricity)
include_directories(external/PEGTL)
include_directories(external/opengm/include)

# remove this from here again, but currently lingeling will not load otherwise
add_library(lingeling STATIC IMPORTED)
set_property(TARGET lingeling PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/external/LP_MP/external/lingeling/")
set_property(TARGET lingeling PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/external/LP_MP/external/lingeling/liblgl.a")

# for opengm
find_package(HDF5 1.8.15 REQUIRED)
include_directories (${HDF5_INCLUDE_DIR})
add_definitions(${HDF5_DEFINITIONS})
message(STATUS ${HDF5_LIBRARIES})
message(STATUS ${HDF5_INCLUDE_DIR})

add_subdirectory(test)
add_subdirectory(src)
