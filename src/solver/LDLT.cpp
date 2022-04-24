

//
// Created by Luecx on 05.04.2022.
//

#include <chrono>
#include "LDLT.h"

Eigen::Matrix<Precision,Eigen::Dynamic,1> simplical_ldlt(const Eigen::SparseMatrix<Precision>& matrix,
                                                           const Eigen::Matrix<Precision,Eigen::Dynamic,1> &b){

    Eigen::Matrix<Precision,Eigen::Dynamic,1> sol(b.size()), t(b.size());
    std::cout << "Attempting to solve matrix: " << b.size() << "x" << b.size() << std::endl;
    Eigen::SimplicialLDLT<Eigen::SparseMatrix<Precision>> solver{};
    auto start = std::chrono::system_clock::now();

    solver.compute(matrix);
    if(solver.info()!=Eigen::Success) {
        // decomposition failed
        std::cout << "decomposition failed" << std::endl;
        return sol;
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Decomposition finished; elapsed time: " << round(diff.count() * 1000) << std::endl;
    start = std::chrono::system_clock::now();

    sol = solver.solve(b);
    if(solver.info()!=Eigen::Success) {
        // solving failed
        std::cout << "solving failed" << std::endl;
        return sol;
    }

    end = std::chrono::system_clock::now();
    diff = end - start;
    std::cout << "Solving finished; elapsed time: " << round(diff.count() * 1000) << std::endl;

    return sol;
}
