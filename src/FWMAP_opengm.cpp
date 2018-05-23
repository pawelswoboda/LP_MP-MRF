
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "LP_FWMAP.hxx"
#include "hdf5_routines.hxx"

using namespace LP_MP;
int main(int argc, char** argv) {
MpRoundingSolver<Solver<LP_tree_FWMAP<FMC_SRMP>,StandardVisitor>> solver(argc,argv);
solver.ReadProblem(ParseOpenGM_DD<Solver<LP_tree_FWMAP<FMC_SRMP>,StandardVisitor>>);
return solver.Solve();
}