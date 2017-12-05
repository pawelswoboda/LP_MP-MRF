#!/usr/bin/python

preamble = """
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
"""

opengm_preamble = """
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "hdf5_routines.hxx"
"""

FMC = [ 
      'FMC_SRMP',
      'FMC_SRMP_T',
      'FMC_MPLP',
      'FMC_SRMP',
      'FMC_MPLP'
      ]

preambles = [
    opengm_preamble,
    opengm_preamble,
    opengm_preamble,
    preamble,
    preamble
    ]

parse_fun = [
      'ParseOpenGM',
      'ParseOpenGM',
      'ParseOpenGM',
      'UaiMrfInput::ParseProblem',
      'UaiMrfInput::ParseProblem'
      ]

file_name = [
      'srmp_opengm.cpp',
      'srmp_tightening_opengm.cpp',
      'mplp_opengm.cpp',
      'srmp_uai.cpp',
      'mplp_uai.cpp',
      ]


main_body = [e[0] + "\nint main(int argc, char* argv[]){\nMpRoundingSolver<Solver<" + e[1] + ",LP,StandardTighteningVisitor>> solver(argc,argv);\nsolver.ReadProblem(" + e[2] + "<Solver<" + e[1] + ",LP,StandardTighteningVisitor>>);\nreturn solver.Solve();\n" for e in zip(preambles,FMC,parse_fun)]
#main_body = ["LP_MP_CONSTRUCT_SOLVER_WITH_INPUT_AND_VISITOR_MP_ROUNDING(" + e[0] + ",TorresaniEtAlInput::" + e[1] + "<" + e[0] + ">,StandardTighteningVisitor);" for e in zip(FMC,parse_fun)]

for e in zip(main_body,file_name):
   f = open(e[1],'w')
   f.write(e[0])
   f.write("\n}\n");
   f.close()
