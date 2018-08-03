#include "test_mrf.hxx"
#include "mrf_uai_input.h"

using namespace LP_MP;

int main()
{
    // MAP-estimation for a model given in uai format
    using FMC = FMC_SRMP;
    using VisitorType = StandardVisitor;
    using SolverType = MpRoundingSolver<Solver<LP<FMC>,VisitorType>>;

    {
        SolverType s(solver_options);
        auto input = mrf_uai_input::parse_string(uai_test_input); 
        s.template GetProblemConstructor<0>().construct(input);
        s.Solve();
        test(std::abs(s.lower_bound() - 0.644) < LP_MP::eps);
    }
    {
        SolverType s(solver_options_2);
        auto input = mrf_uai_input::parse_string(uai_test_input_2); 
        s.template GetProblemConstructor<0>().construct(input);
        s.Solve();
        test(std::abs(s.lower_bound() - 17) < LP_MP::eps);
    }
}
