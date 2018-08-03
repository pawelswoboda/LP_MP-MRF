
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "mrf_uai_input.h"

using namespace LP_MP;
int main(int argc, char** argv) {
MpRoundingSolver<Solver<LP<FMC_SRMP_T>,StandardTighteningVisitor>> solver(argc,argv);
auto input = mrf_uai_input::parse_file(solver.get_input_file());solver.template GetProblemConstructor<0>().construct(input);return solver.Solve();
}