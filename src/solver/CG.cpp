/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFESis free software: you can redistribute it and/or modify                   *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFESis distributed in the hope that it will be useful,                     *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

#include "CG.h"

#include <chrono>
Eigen::VectorXd conjugate_gradient(const Eigen::SparseMatrix<Precision>& matrix, const Eigen::VectorXd& b) {


    Eigen::VectorXd sol(b.size()), t(b.size());

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

    std::cout << "Solved system of equations; elapsed time: " << round(diff.count() * 1000) << std::endl;

    return sol;
}
