project(LP_MP-MRF)
cmake_minimum_required(VERSION 2.8.12)

# C++11
add_compile_options(-std=c++17)

# compiler options
add_definitions(-march=native)

add_subdirectory(external/LP_MP)

add_library(LP_MP-MRF INTERFACE)
target_include_directories(LP_MP-MRF INTERFACE include)
target_include_directories(LP_MP-MRF INTERFACE external)
target_include_directories(LP_MP-MRF INTERFACE external/arboricity)
target_include_directories(LP_MP-MRF INTERFACE external/PEGTL)
target_include_directories(LP_MP-MRF INTERFACE external/opengm/include)
target_link_libraries(LP_MP-MRF INTERFACE LP_MP)

# for opengm
find_package(HDF5 1.8.15 REQUIRED)
include_directories (${HDF5_INCLUDE_DIR})
add_definitions(${HDF5_DEFINITIONS})
message(STATUS ${HDF5_LIBRARIES})
message(STATUS ${HDF5_INCLUDE_DIR})

add_subdirectory(test)
add_subdirectory(src)
