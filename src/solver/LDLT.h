/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFES is free software: you can redistribute it and/or modify                  *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFES is distributed in the hope that it will be useful,                    *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

//
// Created by Luecx on 05.04.2022.
//

#ifndef TOPO_PY_SRC_SOLVER_LDLT_H_
#define TOPO_PY_SRC_SOLVER_LDLT_H_


#include "../eigen/IterativeLinearSolvers"
#include "../eigen/Sparse"
#include "../matrix/DenseMatrix.h"

Eigen::Matrix<Precision,Eigen::Dynamic,1> simplical_ldlt(const Eigen::SparseMatrix<Precision>& matrix,
                                                         const Eigen::Matrix<Precision,Eigen::Dynamic,1> &b);


#endif    // TOPO_PY_SRC_SOLVER_LDLT_H_
