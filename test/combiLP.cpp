#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "solver.hxx"
#include "combiLP.hxx"
#include "hdf5_routines.hxx"
#include "gurobi_interface.hxx"

int main(int argc, char** argv)
{
  using solver_type = LP_MP::MpRoundingSolver<LP_MP::Solver<LP_MP::FMC_SRMP_T,LP_MP::combiLP<DD_ILP::gurobi_interface, LP_MP::LP>,LP_MP::StandardTighteningVisitor>>;
  std::vector<std::string> options = { 
    {""},
    {"-i"}, {"combiLP.uai"},
    //{"-i"}, {"combiLP_test.uai"},
    //{"-i"}, {"test.h5"},
    {"--maxIter"}, {"250"},
    {"--roundingReparametrization"}, {"damped_uniform"},
    {"--standardReparametrization"}, {"anisotropic"},
    //{"--tighten"},
    {"--tightenReparametrization"}, {"damped_uniform"},
    {"--tightenIteration"}, {"100"},
    {"--tightenInterval"}, {"50"},
    {"--tightenConstraintsMax"}, {"10"}
  };
  solver_type solver(options);
  solver.ReadProblem(LP_MP::UaiMrfInput::ParseProblem<LP_MP::Solver<LP_MP::FMC_SRMP_T,LP_MP::combiLP<DD_ILP::gurobi_interface, LP_MP::LP>,LP_MP::StandardTighteningVisitor>>);
  //solver.ReadProblem(LP_MP::ParseOpenGM<LP_MP::Solver<LP_MP::FMC_SRMP_T,LP_MP::combiLP<DD_ILP::gurobi_interface, LP_MP::LP>,LP_MP::StandardTighteningVisitor>>);

  solver.Solve(); 
}
