
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "hdf5_routines.hxx"

int main(int argc, char* argv[]){
MpRoundingSolver<Solver<FMC_MPLP,LP,StandardTighteningVisitor>> solver(argc,argv);
solver.ReadProblem(ParseOpenGM<Solver<FMC_MPLP,LP,StandardTighteningVisitor>>);
return solver.Solve();

}
