
#include "graphical_model.h" 
#include "visitors/standard_visitor.hxx" 
#include "LP_FWMAP.hxx"
#include "mrf_opengm_input.h"

using namespace LP_MP;
int main(int argc, char** argv) {
MpRoundingSolver<Solver<LP_tree_FWMAP<FMC_SRMP>,StandardVisitor>> solver(argc,argv);
auto input = mrf_opengm_input::parse_file(solver.get_input_file());solver.template GetProblemConstructor<0>().construct(input);auto trees = solver.template GetProblemConstructor<0>().compute_forest_cover();for(auto& tree : trees) { solver.GetLP().add_tree(tree); }return solver.Solve();
}