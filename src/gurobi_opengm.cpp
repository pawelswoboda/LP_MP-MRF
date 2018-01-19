#include "graphical_model.h"
#include "LP_external_interface.hxx" 
#include "gurobi_interface.hxx"
#include "visitors/standard_visitor.hxx"
#include "hdf5_routines.hxx"

using namespace LP_MP;

int main(int argc, char** argv) 
{
  using solver_type = Solver<FMC_SRMP,LP_external_solver<DD_ILP::gurobi_interface,LP>,StandardVisitor>;
  solver_type solver(argc, argv);
  solver.ReadProblem(ParseOpenGM<solver_type>);
  solver.GetLP().solve(); 
} 
