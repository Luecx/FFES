//
// Created by finne on 7/28/2020.
//

#include <cassert>
#include <algorithm>
#include <emmintrin.h>
#include "SparseMatrix.h"


SparseMatrix::SparseMatrix(int mP, int nP) : Matrix(mP, nP) {
    row_ptr.resize(this->getM() + 1);
}

SparseMatrix::SparseMatrix(const SparseMatrix &other) : Matrix(other) {
    this->values = other.values;
    this->row_ptr = other.row_ptr;
    this->col_index = other.col_index;
}

SparseMatrix::SparseMatrix(SparseMatrix&& other) : Matrix(other) {
    this->values = std::move(other.values);
    this->row_ptr = std::move(other.row_ptr);
    this->col_index = std::move(other.col_index);
}

SparseMatrix::SparseMatrix(ListedMatrix &hashMatrix) : Matrix(hashMatrix){
    this->values = *(new std::vector<Precision>());
    this->row_ptr = *(new std::vector<int>());
    this->col_index = *(new std::vector<int>());
    
    row_ptr.push_back(0);
    int total = 0;
    for (int i = 0; i < hashMatrix.getM(); i++) {
        std::vector<RowEntry> map = hashMatrix.getRow(i);
        total += map.size();
        row_ptr.emplace_back(total);
        
        std::sort(map.begin(), map.end());
        for (int p_n = 0; p_n < (int)map.size(); p_n++) {
            col_index.emplace_back(map.at(p_n).column);
            values.emplace_back(map.at(p_n).value);
        }
    }
}



void SparseMatrix::reset() {
    this->values.clear();
    this->row_ptr.clear();
    this->col_index.clear();
    for (int i = 0; i < this->getM() + 1; i++) {
        row_ptr.push_back(0);
    }
}

void SparseMatrix::set(int p_m, int p_n, Precision val) {
    int insertIndex = row_ptr.at(p_m + 1);
    for (int i = row_ptr.at(p_m); i < row_ptr.at(p_m + 1); i++) {
        if (col_index.at(i) < p_n) {
            continue;
        } else if (col_index.at(i) == p_n) {
            values.at(i) = val;
            return;
        } else if (col_index.at(i) > p_n) {
            insertIndex = i;
            break;
        }
    }
    col_index.insert(col_index.begin() + insertIndex, p_n);
    values.insert(values.begin() + insertIndex, val);
    for (int i = p_m + 1; i < getM() + 1; i++) {
        row_ptr.at(i) = row_ptr.at(i) + 1;
    }
}

Precision &SparseMatrix::at(int p_m, int p_n) {
    for (int i = row_ptr.at(p_m); i < row_ptr.at(p_m + 1); i++) {
        if (col_index.at(i) == p_n) {
            return values.at(i);
        }
    }
    
    set(p_m,p_n,0);
    return at(p_m,p_n);
}

Precision SparseMatrix::get(int p_m, int p_n) const {
    for (int i = row_ptr.at(p_m); i < row_ptr.at(p_m + 1); i++) {
        if (col_index.at(i) == p_n) {
            return values.at(i);
        }
    }
    return 0;
}

//DenseVector &SparseMatrix::mul(DenseVector &vec) {
//    DenseVector *out = new DenseVector(getM());
//
//
//    for(int i = 0; i < getM(); i++){
//
//
//        Precision sum = 0;
//        for (int n = row_ptr.at(i); n < row_ptr.at(i + 1); n++) {
//            sum += vec.at(col_index.at(n)) * this->values.at(n);
//        }
//        out->set(i,sum);
//    }
//
//
//    return *out;
//}


void SparseMatrix::printCSRFormat() {
    std::cout << ("values    :");
    for (int i = 0; i < (int)values.size(); i++) {
        std::cout << std::setw(5) << values.at(i);
    }
    std::cout << "\ncol_index :";
    for (int i = 0; i < (int) col_index.size(); i++) {
        std::cout << std::setw(5) << col_index.at(i);
    }
    std::cout << "\nrow_ptr.  :";
    for (int i = 0; i < (int)row_ptr.size(); i++) {
        std::cout << std::setw(5) << row_ptr.at(i);
    }
    std::cout << std::endl;
}

uint64_t SparseMatrix::mem() {
    return
     2 * sizeof(int) +
     1 * sizeof(std::vector<Precision>) + (sizeof(Precision) * values.size()) +
     1 * sizeof(std::vector<int>) + (sizeof(int) * row_ptr.size()) +
     1 * sizeof(std::vector<int>) + (sizeof(int) * col_index.size());
}

const std::vector<Precision> &SparseMatrix::getValues() const {
    return values;
}

const std::vector<int> &SparseMatrix::getColIndex() const {
    return col_index;
}

const std::vector<int> &SparseMatrix::getRowPtr() const {
    return row_ptr;
}

DenseMatrix SparseMatrix::operator*(const DenseMatrix& other) const {
    ASSERT(other.getM() == this->n);
    DenseMatrix res{m, other.getN()};
    for(int p_m = 0; p_m < m; p_m++){
        for(int p_n = 0; p_n < other.getN(); p_n++){

            double sum = 0;
            for (int j = row_ptr.at(p_m); j < row_ptr.at(p_m + 1); j++) {
                sum += other(col_index[j], p_n) * this->values.at(j);
            }
            res(p_m, p_n) = sum;
        }
    }
    return res;
}
