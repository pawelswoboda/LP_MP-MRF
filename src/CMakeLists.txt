add_library(mrf_uai_input mrf_uai_input.cpp)
target_link_libraries(mrf_uai_input LP_MP-MRF) 

add_library(mrf_opengm_input mrf_opengm_input.cpp)
target_link_libraries(mrf_opengm_input LP_MP-MRF HDF5) 

SET(UAI_SOURCE_FILES
   mplp_uai.cpp
   srmp_uai.cpp
   srmp_uai_tightening.cpp
   )

SET(OPENGM_SOURCE_FILES
   mplp_opengm.cpp
   srmp_opengm.cpp
   srmp_opengm_tightening.cpp
   )

foreach( source_file ${UAI_SOURCE_FILES} )
   string( REPLACE ".cpp" "" executable_file ${source_file} )
   add_executable( ${executable_file} ${source_file} )
   target_link_libraries( ${executable_file} LP_MP-MRF DD_ILP m stdc++ pthread lingeling mrf_uai_input ) 
endforeach( source_file ${SOURCE_FILES} )

foreach( source_file ${OPENGM_SOURCE_FILES} )
   string( REPLACE ".cpp" "" executable_file ${source_file} )
   add_executable( ${executable_file} ${source_file} ${headers} ${sources})
   target_link_libraries( ${executable_file} LP_MP-MRF m stdc++ pthread lingeling mrf_opengm_input ) 
endforeach( source_file ${SOURCE_FILES} )

if(WITH_GUROBI)
  add_executable( srmp_uai_combiLP srmp_uai_combiLP.cpp )
  target_link_libraries( srmp_uai_combiLP LP_MP-MRF DD_ILP GUROBI m stdc++ pthread lingeling ) 

  add_executable( srmp_opengm_combiLP srmp_opengm_combiLP.cpp )
  target_link_libraries( srmp_opengm_combiLP LP_MP-MRF DD_ILP GUROBI m stdc++ pthread lingeling HDF5 ) 

  add_executable( gurobi_opengm gurobi_opengm.cpp )
  target_link_libraries( gurobi_opengm LP_MP-MRF DD_ILP GUROBI m stdc++ pthread lingeling HDF5 )

  add_executable( gurobi_uai gurobi_uai.cpp )
  target_link_libraries( gurobi_uai LP_MP-MRF DD_ILP GUROBI m stdc++ pthread lingeling )
endif()

add_executable(FWMAP_opengm FWMAP_opengm.cpp)
target_link_libraries(FWMAP_opengm HDF5 LP_MP-MRF m stdc++ pthread lingeling FW-MAP arboricity mrf_opengm_input)

add_executable(FWMAP_uai FWMAP_uai.cpp)
target_link_libraries(FWMAP_uai LP_MP-MRF m stdc++ pthread lingeling FW-MAP arboricity mrf_uai_input)

add_executable(conic_bundle_opengm conic_bundle_opengm.cpp)
target_link_libraries(conic_bundle_opengm HDF5 LP_MP-MRF m stdc++ pthread lingeling cb arboricity mrf_opengm_input)

add_executable(conic_bundle_uai conic_bundle_uai.cpp)
target_link_libraries(conic_bundle_uai LP_MP-MRF m stdc++ pthread lingeling cb arboricity mrf_uai_input)
