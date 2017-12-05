SET(SOURCE_FILES
   potts_factor.cpp
   simplex.cpp
   simplex_marginalization.cpp
   graphical_model.cpp
   cycle_inequalities.cpp
   )

foreach( source_file ${SOURCE_FILES} )
   string( REPLACE ".cpp" "" executable_file ${source_file} )
   set(TARGET_NAME ${executable_file})
   add_executable( ${executable_file} ${source_file} )
   target_link_libraries( ${executable_file} LP_MP-MRF lingeling m stdc++ pthread )
endforeach( source_file ${SOURCE_FILES} )

