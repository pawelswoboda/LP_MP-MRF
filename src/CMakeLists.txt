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
   target_link_libraries( ${executable_file} LP_MP-MRF DD_ILP GUROBI m stdc++ pthread lingeling ) 
endforeach( source_file ${SOURCE_FILES} )

foreach( source_file ${OPENGM_SOURCE_FILES} )
   string( REPLACE ".cpp" "" executable_file ${source_file} )
   add_executable( ${executable_file} ${source_file} ${headers} ${sources})
   target_link_libraries( ${executable_file} HDF5 LP_MP-MRF GUROBI m stdc++ pthread lingeling ) 
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
target_link_libraries(FWMAP_opengm HDF5 LP_MP-MRF m stdc++ pthread lingeling FW-MAP GUROBI arboricity)

add_executable(FWMAP_uai FWMAP_uai.cpp)
target_link_libraries(FWMAP_uai LP_MP-MRF m stdc++ pthread lingeling FW-MAP GUROBI arboricity)
