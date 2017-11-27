# LP_MP-MRF
Dual decomposition based Markov-Random-Fields solvers.

## Available solvers
* SRMP [1]
* MPLP [2]

Also, tightening with cycle inequalities [3] is supported.

## Input formats
Input is supported in the [UAI](http://www.hlt.utdallas.edu/~vgogate/uai14-competition/modelformat.html) format as well as in the hdf5-based [opengm](https://github.com/opengm/opengm/) format.

## Installation
Type `git clone https://github.com/pawelswoboda/LP_MP-MRF.git`, then `cd LP_MP-MRF` and initialize dependencies via `git submodule update --init --recursive`.
Then type `cmake .` to configure and `make` to build.

## References
* [1]: `V. Kolmogorov. A New Look at Reweighted Message Passing. IEEE Trans. Pattern Anal. Mach. Intell., 37(5):919–930, 2015.`
* [2]: `A. Globerson and T. S. Jaakkola. Fixing max-product: Convergent message passing algorithms for MAP LP-relaxations.  In NIPS, pages 553–560, 2007.`
* [3]: `D. Sontag, D. K. Choe, and Y. Li. Efficiently searching for frustrated cycles in map inference. In UAI , pages 795–804.  AUAI Press, 2012.`
