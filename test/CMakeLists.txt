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
   target_link_libraries( ${executable_file} LP_MP-MRF DD_ILP GUROBI lingeling m stdc++ pthread )
   add_test(${executable_file} ${executable_file})
endforeach( source_file ${SOURCE_FILES} )

if(WITH_GUROBI)
  add_executable(combiLP combiLP.cpp)
  target_link_libraries(combiLP LP_MP-MRF m stdc++ pthread GUROBI HDF5 lingeling) #somehow currently needs to be linked against. Remove this!
  add_test(combiLP combiLP)

  add_executable(dd_ilp_gurobi dd_ilp_gurobi.cpp)
  target_link_libraries(dd_ilp_gurobi LP_MP-MRF m stdc++ pthread GUROBI HDF5 lingeling) 
  add_test(dd_ilp_gurobi dd_ilp_gurobi)
endif()
