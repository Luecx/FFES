//
// Created by Luecx on 08.09.2021.
//

#include "CG.h"

#include <chrono>
DenseMatrix conjugate_gradient(const SparseMatrix& matrix, const DenseMatrix& b,
                               Precision tolerance, DenseMatrix x) {

    auto start = std::chrono::system_clock::now();

    if(x.getM() != matrix.getM()){
        x = DenseMatrix {matrix.getM(), 1};
    }


    DenseMatrix C{matrix.getM(),1};
    for(int i = 0; i < C.getM(); i++){
        C(i,0) = 1 / matrix(i,i);
    }
    DenseMatrix r = b - matrix * x;
    DenseMatrix z = C.hadamard(r);
    DenseMatrix p = z;
    for(int iteration = 0; iteration < matrix.getM() * 100; iteration++){

        auto Ap = matrix * p;

        Precision r_dot_z = r.dot(z);
        Precision alpha   = r_dot_z / p.dot(Ap);
        x += p * alpha;
        r -= Ap * alpha;

        std::cout << "  iteration: " << std::setw(4) << iteration << " res: " << sqrt(r.dot(r)) << std::endl;
        if(r.dot(r) < tolerance * tolerance){
            break;
        }

        z  = C.hadamard(r);

        Precision beta = r.dot(z) / r_dot_z;
        p = z + p * beta;
    }



//    DenseMatrix r = b - matrix * x;
//    DenseMatrix p = r;
//    auto rsold = r.dot(r);
//    for(int iteration = 0; iteration < matrix.getM()* 100; iteration++){
//
//        auto Ap = matrix * p;
//        auto alpha = rsold / (p.dot(Ap));
//        x = x + p  * alpha;
//        r = r - Ap * alpha;
//        auto rsnew = r.dot(r);
//
//        std::cout << "  iteration: " << std::setw(4) << iteration << " res: " << sqrt(rsnew) << std::endl;
//        if(rsnew < tolerance * tolerance){
//            break;
//        }
//
//        p = r + p * (rsnew / rsold) ;
//        rsold = rsnew;
//    }


    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = end - start;

    std::cout << "Solved system of equations; elapsed time: " << round(diff.count() * 1000) << std::endl;

    return x;
}
