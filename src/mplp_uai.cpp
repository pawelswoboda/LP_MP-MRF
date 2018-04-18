
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"

using namespace LP_MP;
int main(int argc, char** argv) {
MpRoundingSolver<Solver<FMC_MPLP,LP<FMC_MPLP>,StandardVisitor>> solver(argc,argv);
solver.ReadProblem(UaiMrfInput::ParseProblem<Solver<FMC_MPLP,LP<FMC_MPLP>,StandardVisitor>>);
return solver.Solve();
}