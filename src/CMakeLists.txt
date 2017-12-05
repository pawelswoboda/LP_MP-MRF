SET(UAI_SOURCE_FILES
   mplp_uai.cpp
   srmp_uai.cpp
   )

SET(OPENGM_SOURCE_FILES
   mplp_opengm.cpp
   srmp_opengm.cpp
   srmp_tightening_opengm.cpp
   )

foreach( source_file ${UAI_SOURCE_FILES} )
   string( REPLACE ".cpp" "" executable_file ${source_file} )
   add_executable( ${executable_file} ${source_file} )
   target_link_libraries( ${executable_file} LP_MP-MRF m stdc++ pthread lingeling ) 
endforeach( source_file ${SOURCE_FILES} )

foreach( source_file ${OPENGM_SOURCE_FILES} )
   string( REPLACE ".cpp" "" executable_file ${source_file} )
   add_executable( ${executable_file} ${source_file} ${headers} ${sources})
   target_link_libraries( ${executable_file} HDF5 LP_MP-MRF m stdc++ pthread lingeling ) 
endforeach( source_file ${SOURCE_FILES} )

