

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
