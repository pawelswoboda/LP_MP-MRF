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

add_subdirectory(external/arboricity)

# for opengm
find_package(HDF5 1.8.15 REQUIRED)
add_library(HDF5 INTERFACE)
target_include_directories(HDF5 INTERFACE ${HDF5_INCLUDE_DIR})
target_link_libraries(HDF5 INTERFACE ${HDF5_LIBRARIES})
target_compile_definitions(HDF5 INTERFACE ${HDF5_DEFINITIONS})
message(STATUS ${HDF5_LIBRARIES})
message(STATUS ${HDF5_INCLUDE_DIR})

add_subdirectory(test)
enable_testing()
add_subdirectory(src)

option(BUILD_DOC "Build documentation" OFF)
if(BUILD_DOC)
  find_package(Doxygen REQUIRED)
  configure_file(Doxyfile.in Doxyfile @ONLY)
  add_custom_target(docs-mrf
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
  if(NOT TARGET docs)
    add_custom_target(docs ALL)
  endif()
  add_dependencies(docs docs-mrf)
endif()
