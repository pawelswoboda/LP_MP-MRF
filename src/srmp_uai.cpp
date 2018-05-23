
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"

using namespace LP_MP;
int main(int argc, char** argv) {
MpRoundingSolver<Solver<LP<FMC_SRMP>,StandardVisitor>> solver(argc,argv);
solver.ReadProblem(UaiMrfInput::ParseProblem<Solver<LP<FMC_SRMP>,StandardVisitor>>);
return solver.Solve();
}