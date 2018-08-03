add_executable(potts_factor potts_factor.cpp)
target_link_libraries(potts_factor LP_MP-MRF m stdc++ pthread lingeling)
add_test(potts_factor potts_factor)

add_executable(simplex simplex.cpp)
target_link_libraries(simplex LP_MP-MRF m stdc++ pthread lingeling)
add_test(simplex simplex)

add_executable(simplex_marginalization simplex_marginalization.cpp)
target_link_libraries(simplex_marginalization LP_MP-MRF m stdc++ pthread lingeling)
add_test(simplex_marginalization simplex_marginalization)

add_executable(cycle_inequalities cycle_inequalities.cpp)
target_link_libraries(cycle_inequalities LP_MP-MRF m stdc++ pthread lingeling)
add_test(cycle_inequalities cycle_inequalities)

add_executable(mrf_tree_decomposition mrf_tree_decomposition.cpp)
target_link_libraries(mrf_tree_decomposition LP_MP-MRF m stdc++ pthread lingeling FW-MAP arboricity CONIC_BUNDLE)
add_test(mrf_tree_decomposition mrf_tree_decomposition)

add_executable(mrf_tree mrf_tree.cpp)
target_link_libraries(mrf_tree LP_MP-MRF m stdc++ pthread lingeling arboricity)
add_test(mrf_tree mrf_tree)

add_executable(mrf_read_uai_file mrf_read_uai_file.cpp)
target_link_libraries(mrf_read_uai_file LP_MP-MRF m stdc++ pthread lingeling mrf_uai_input)
add_test(mrf_read_uai_file mrf_read_uai_file)

add_executable(mrf_sat_rounding mrf_sat_rounding.cpp)
target_link_libraries(mrf_sat_rounding LP_MP-MRF m stdc++ pthread lingeling)
add_test(mrf_sat_rounding mrf_sat_rounding)

add_executable(mrf_tightening mrf_tightening.cpp)
target_link_libraries(mrf_tightening LP_MP-MRF m stdc++ pthread lingeling)
add_test(mrf_tightening mrf_tightening)

if(WITH_GUROBI)
  add_executable(combiLP combiLP.cpp)
  target_link_libraries(combiLP LP_MP-MRF m stdc++ pthread GUROBI HDF5 lingeling) #somehow currently needs to be linked against. Remove this!
  add_test(combiLP combiLP)

  add_executable(dd_ilp_gurobi dd_ilp_gurobi.cpp)
  target_link_libraries(dd_ilp_gurobi LP_MP-MRF m stdc++ pthread GUROBI HDF5 lingeling) 
  add_test(dd_ilp_gurobi dd_ilp_gurobi)
endif()
