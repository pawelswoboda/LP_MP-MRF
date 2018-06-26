#include "test_mrf.hxx"

using namespace LP_MP;

int main()
{
    // MAP-estimation for a model given in uai format
    using FMC = FMC_SRMP;
    using VisitorType = StandardVisitor;
    using SolverType = MpRoundingSolver<Solver<LP<FMC>,VisitorType>>;

    {
        SolverType s(solver_options);
        UaiMrfInput::ParseString<SolverType,0>(uai_test_input, s); 
        s.Solve();
        test(std::abs(s.lower_bound() - 0.644) < LP_MP::eps);
    }
    {
        SolverType s(solver_options_2);
        UaiMrfInput::ParseString<SolverType,0>(uai_test_input_2, s); 
        s.Solve();

        test(std::abs(s.lower_bound() - 17) < LP_MP::eps);
    }
}
