//
// Created by Luecx on 08.09.2021.
//

#ifndef FEM_SRC_SOLVER_CG_H_
#define FEM_SRC_SOLVER_CG_H_

#include "../matrix/DenseMatrix.h"
#include "../matrix/SparseMatrix.h"

DenseMatrix conjugate_gradient(const SparseMatrix& matrix, const DenseMatrix& vector, Precision tolerance = 1e-12, DenseMatrix x = DenseMatrix(1,1));



#endif    // FEM_SRC_SOLVER_CG_H_
