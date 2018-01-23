
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "LP_conic_bundle.hxx"
#include "hdf5_routines.hxx"

using namespace LP_MP;
int main(int argc, char** argv) {
MpRoundingSolver<Solver<FMC_SRMP,LP_conic_bundle,StandardVisitor>> solver(argc,argv);
solver.ReadProblem(ParseOpenGM_DD<Solver<FMC_SRMP,LP_conic_bundle,StandardVisitor>>);
return solver.Solve();
}