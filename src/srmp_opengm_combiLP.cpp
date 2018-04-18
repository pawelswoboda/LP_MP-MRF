
#include "graphical_model.h"
#include "combiLP.hxx"
#include "gurobi_interface.hxx"
#include "visitors/standard_visitor.hxx"
#include "hdf5_routines.hxx"

using namespace LP_MP;
int main(int argc, char** argv) {
MpRoundingSolver<Solver<FMC_SRMP,combiLP<DD_ILP::gurobi_interface, LP<FMC_SRMP>>,StandardVisitor>> solver(argc,argv);
solver.ReadProblem(ParseOpenGM<Solver<FMC_SRMP,combiLP<DD_ILP::gurobi_interface, LP<FMC_SRMP>>,StandardVisitor>>);
return solver.Solve();
}