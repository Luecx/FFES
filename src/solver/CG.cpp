

#include "CG.h"

#include <chrono>
Eigen::Matrix<Precision,Eigen::Dynamic,1> conjugate_gradient(const Eigen::SparseMatrix<Precision>& matrix,
                                   const Eigen::Matrix<Precision,Eigen::Dynamic,1>& b) {

    std::cout << matrix << std::endl;
    std::cout << b << std::endl;

    Eigen::Matrix<Precision,Eigen::Dynamic,1> sol(b.size()), t(b.size());
    std::cout << "Attempting to solve matrix: " << b.size() << "x" << b.size() << std::endl;

    Eigen::ConjugateGradient<
        Eigen::SparseMatrix<Precision>,
        Eigen::Lower|Eigen::Upper ,
        Eigen::IncompleteCholesky<Precision>> solver;

    solver.compute(matrix);

    auto start = std::chrono::system_clock::now();
    if(solver.info()!=Eigen::Success) {
        // decomposition failed
        std::cout << "decomposition failed" << std::endl;
        return sol;
    }
    sol = solver.solve(b);
    if(solver.info()!=Eigen::Success) {
        // solving failed
        std::cout << "solving failed" << std::endl;
        return sol;
    }

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = end - start;

    std::cout << "Solved system of equations; elapsed time: " << round(diff.count() * 1000)
              << " estimated error: " << solver.error() <<  std::endl;
    std::cout << sol << std::endl;
    std::cout << matrix * sol - b << std::endl;

    return sol;
}
