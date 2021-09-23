

#ifndef FEM_SRC_SOLVER_CG_H_
#define FEM_SRC_SOLVER_CG_H_

#include "../eigen/IterativeLinearSolvers"
#include "../eigen/Sparse"
#include "../matrix/DenseMatrix.h"

Eigen::Matrix<Precision,Eigen::Dynamic,1> conjugate_gradient(const Eigen::SparseMatrix<Precision>& matrix,
                                                             const Eigen::Matrix<Precision,Eigen::Dynamic,1> &b);



#endif    // FEM_SRC_SOLVER_CG_H_
