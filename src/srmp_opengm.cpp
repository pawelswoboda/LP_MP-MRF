
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "hdf5_routines.hxx"

using namespace LP_MP;
int main(int argc, char** argv) {
MpRoundingSolver<Solver<FMC_SRMP,LP<FMC_SRMP>,StandardVisitor>> solver(argc,argv);
solver.ReadProblem(ParseOpenGM<Solver<FMC_SRMP,LP<FMC_SRMP>,StandardVisitor>>);
return solver.Solve();
}