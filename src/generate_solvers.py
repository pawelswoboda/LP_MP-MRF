#!/usr/bin/python

from collections import namedtuple
solver = namedtuple("solver_info", "preamble FMC LP parse_fun filename visitor")

preamble = """
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
"""

opengm_preamble = """
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "hdf5_routines.hxx"
"""

combiLP_preamble = """
#include "graphical_model.h"
#include "combiLP.hxx"
#include "gurobi_interface.hxx"
#include "visitors/standard_visitor.hxx"
"""

opengm_combiLP_preamble = """
#include "graphical_model.h"
#include "combiLP.hxx"
#include "gurobi_interface.hxx"
#include "visitors/standard_visitor.hxx"
#include "hdf5_routines.hxx"
"""

dd_preamble = """
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "LP_FWMAP.hxx"
"""

dd_preamble_opengm = """
#include "graphical_model.h"
#include "visitors/standard_visitor.hxx"
#include "LP_FWMAP.hxx"
#include "hdf5_routines.hxx"
"""

solvers = [
    solver(opengm_preamble, 'FMC_SRMP', 'LP', 'ParseOpenGM', 'srmp_opengm.cpp', 'StandardVisitor'),
    solver(opengm_preamble, 'FMC_SRMP_T', 'LP', 'ParseOpenGM', 'srmp_opengm_tightening.cpp', 'StandardTighteningVisitor'),
    solver(opengm_preamble, 'FMC_MPLP', 'LP', 'ParseOpenGM', 'mplp_opengm.cpp', 'StandardVisitor'),
    solver(preamble, 'FMC_SRMP', 'LP', 'UaiMrfInput::ParseProblem', 'srmp_uai.cpp', 'StandardVisitor'),
    solver(preamble, 'FMC_SRMP_T', 'LP', 'UaiMrfInput::ParseProblem', 'srmp_uai_tightening.cpp', 'StandardTighteningVisitor'),
    solver(preamble, 'FMC_MPLP', 'LP', 'UaiMrfInput::ParseProblem', 'mplp_uai.cpp', 'StandardVisitor'),
    solver(combiLP_preamble, 'FMC_SRMP', 'combiLP<DD_ILP::gurobi_interface, LP>', 'UaiMrfInput::ParseProblem', 'srmp_uai_combiLP.cpp', 'StandardVisitor'),
    solver(opengm_combiLP_preamble, 'FMC_SRMP', 'combiLP<DD_ILP::gurobi_interface, LP>', 'ParseOpenGM', 'srmp_opengm_combiLP.cpp', 'StandardVisitor'),
    solver(dd_preamble, 'FMC_SRMP', 'LP_tree_FWMAP', 'UaiMrfInput::ParseProblemDD', 'FWMAP_uai.cpp', 'StandardVisitor'),
    solver(dd_preamble_opengm, 'FMC_SRMP', 'LP_tree_FWMAP', 'ParseOpenGM_DD', 'FWMAP_opengm.cpp', 'StandardVisitor')
    ]


for e in solvers:
   f = open(e.filename,'w')
   f.write(e.preamble)
   f.write("\nusing namespace LP_MP;\n")
   f.write("int main(int argc, char** argv) {\nMpRoundingSolver<Solver<")
   f.write(e.FMC + "," + e.LP + "," + e.visitor + ">> solver(argc,argv);\n")
   f.write("solver.ReadProblem(" + e.parse_fun + "<Solver<" + e.FMC + "," + e.LP + "," + e.visitor + ">>);\n")
   f.write("return solver.Solve();\n}")
