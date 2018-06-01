#include "test_mrf.hxx"

using namespace LP_MP;

int main()
{
    // MAP-estimation for a model given in uai format
    using FMC = FMC_SRMP;
    using VisitorType = StandardVisitor;
    using SolverType = MpRoundingSolver<Solver<LP<FMC>,VisitorType>>;

    SolverType s(solver_options);
    UaiMrfInput::ParseString<SolverType,0>(uai_test_input, s); 
    s.Solve();

    test(std::abs(s.lower_bound() - 0.564) < LP_MP::eps); // is this actually correct?
}
