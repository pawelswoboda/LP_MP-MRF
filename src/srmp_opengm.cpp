
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "mrf_opengm_input.h"

using namespace LP_MP;
int main(int argc, char** argv) {
MpRoundingSolver<Solver<LP<FMC_SRMP>,StandardVisitor>> solver(argc,argv);
auto input = mrf_opengm_input::parse_file(solver.get_input_file());solver.template GetProblemConstructor<0>().construct(input);return solver.Solve();
}