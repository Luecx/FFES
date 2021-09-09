//
// Created by finne on 7/28/2020.
//

#ifndef MATLIB_SPARSEMATRIX_H
#define MATLIB_SPARSEMATRIX_H

#include "DenseMatrix.h"
#include "ListedMatrix.h"
#include "Matrix.h"

#include <vector>

class SparseMatrix : public Matrix{
    
    private:
    
        std::vector<Precision> values;
        std::vector<int> col_index;
        std::vector<int> row_ptr;
    
    public:
        
        SparseMatrix(int mP, int nP);
        
        SparseMatrix() = delete;
        
        SparseMatrix(const SparseMatrix &other);

        SparseMatrix(SparseMatrix &&other);

        SparseMatrix(ListedMatrix &hashMatrix);

        void printCSRFormat();
        
        uint64_t mem() override;
        
        void reset() override;
        
        void set(int p_m, int p_n, Precision val) override;
        
        Precision &at(int p_m, int p_n)  override;

        Precision get(int p_m, int p_n) const override;

        const std::vector<Precision> &getValues() const;
        
        const std::vector<int> &getColIndex() const;
        
        const std::vector<int> &getRowPtr() const;

        DenseMatrix operator*(const DenseMatrix& other) const;

        
        
};


#endif //MATLIB_SPARSEMATRIX_H
