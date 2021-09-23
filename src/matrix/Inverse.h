
#ifndef FEM_SRC_MATRIX_INVERSE_H_
#define FEM_SRC_MATRIX_INVERSE_H_

#include "QuickMatrix.h"

template<int M>
inline QuickMatrix<M, M> inverse(QuickMatrix<M, M> A, Precision& determinant) {
    QuickMatrix<M, M> P {};
    P.identity();

    determinant = 1;
    // lower triangular part
    for (int i = 0; i < M; i++) {

        // pivoting
        int pivot = i;
        for (int j = i + 1; j < M; j++) {
            if (std::abs(A(j, i)) > std::abs(A(pivot, i))) {
                pivot = j;
            }
        }

        A.swapRows(i, pivot);
        P.swapRows(i, pivot);

        if (determinant == 0) {
            return P;
        }

        // elimination
        for (int j = i + 1; j < M; j++) {
            Precision s1 = A(j, i) / A(i, i);

            for (int k = 0; k < M; k++) {
                A(j, k) = A(j, k) - s1 * A(i, k);
                P(j, k) = P(j, k) - s1 * P(i, k);
            }
        }
        determinant *= A(i, i);
    }

    // upper triangular part
    for (int i = M - 1; i >= 0; i--) {

        for (int j = 0; j < i; j++) {
            Precision s1 = A(j, i) / A(i, i);

            for (int k = 0; k < M; k++) {
                A(j, k) = A(j, k) - s1 * A(i, k);
                P(j, k) = P(j, k) - s1 * P(i, k);
            }
        }
    }

    // correct scalings
    for (int j = 0; j < M; j++) {
        for (int k = 0; k < M; k++) {
            P(j, k) = P(j, k) / A(j, j);
        }
    }
    return P;
}

inline QuickMatrix<3, 3> inverse3x3(QuickMatrix<3, 3> A, Precision& determinant){

    determinant =
          A(0,0) * A(1,1) * A(2,2)
        + A(0,1) * A(1,2) * A(2,0)
        + A(0,2) * A(1,0) * A(2,1)
        - A(0,0) * A(1,2) * A(2,1)
        - A(0,1) * A(1,0) * A(2,2)
        - A(0,2) * A(1,1) * A(2,0);

    if(determinant == 0) return A;
    QuickMatrix<3, 3> inverse{};
//    inverse(0,0) =  1/determinant * (A(1,1) * A(2,2) - A(1,2) * A(2,1));
//    inverse(1,0) = -1/determinant * (A(0,1) * A(2,2) - A(0,2) * A(2,1));
//    inverse(2,0) =  1/determinant * (A(0,1) * A(1,2) - A(0,2) * A(1,1));
//
//    inverse(0,1) = -1/determinant * (A(1,0) * A(2,2) - A(1,2) * A(2,0));
//    inverse(1,1) =  1/determinant * (A(0,0) * A(2,2) - A(0,2) * A(2,0));
//    inverse(2,1) = -1/determinant * (A(0,0) * A(1,2) - A(0,2) * A(1,0));
//
//    inverse(0,2) =  1/determinant * (A(1,0) * A(2,1) - A(1,1) * A(2,0));
//    inverse(1,2) = -1/determinant * (A(0,0) * A(2,1) - A(2,0) * A(0,1));
//    inverse(2,2) =  1/determinant * (A(0,0) * A(1,1) - A(0,1) * A(1,0));
    inverse(0,0) =  1/determinant * (A(1,1)*A(2,2)-A(1,2)*A(2,1));
    inverse(1,0) =  1/determinant * (A(1,2)*A(2,0)-A(1,0)*A(2,2));
    inverse(2,0) =  1/determinant * (A(1,0)*A(2,1)-A(1,1)*A(2,0));

    inverse(0,1) =  1/determinant * (A(0,2)*A(2,1)-A(0,1)*A(2,2));
    inverse(1,1) =  1/determinant * (A(0,0)*A(2,2)-A(0,2)*A(2,0));
    inverse(2,1) =  1/determinant * (A(0,1)*A(2,0)-A(0,0)*A(2,1));

    inverse(0,2) =  1/determinant * (A(0,1)*A(1,2)-A(0,2)*A(1,1));
    inverse(1,2) =  1/determinant * (A(0,2)*A(1,0)-A(0,0)*A(1,2));
    inverse(2,2) =  1/determinant * (A(0,0)*A(1,1)-A(0,1)*A(1,0));

    return inverse;
}

inline QuickMatrix<2, 2> inverse2x2(QuickMatrix<2, 2> A, Precision& determinant){

    determinant =
           A(0,0) * A(1,1)
         - A(1,0) * A(0,1);

    if(determinant == 0) return A;
    QuickMatrix<2,2> inverse{};
    inverse(0,0) =   1/determinant * A(1,1);
    inverse(1,1) =   1/determinant * A(0,0);
    inverse(0,1) =  -1/determinant * A(1,0);
    inverse(1,0) =  -1/determinant * A(0,1);

    return inverse;
}

#endif    // FEM_SRC_MATRIX_INVERSE_H_
