//
// Created by finn on 05.06.20.
//

#ifndef MATLIB_MATRIX_H
#define MATLIB_MATRIX_H

#include "../assert/Assert.h"
#include "../core/defs.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <ostream>

class Matrix {

public:

    const int m, n;

    Matrix(int m_p, int n_p) : m(m_p), n(n_p) {}

    int getM() const {
        return m;
    }
    int getN() const {
        return n;
    }

    virtual uint64_t mem() = 0;

    virtual void reset() = 0;

    virtual void set(int m, int n, Precision val) = 0;
    virtual Precision &at(int p_m, int p_n) = 0;
    virtual Precision get(int p_m, int p_n) const = 0;

    inline Precision &operator()(int p_m, int p_n) {
        ASSERT(p_m < m && p_n < n && m >= 0 && n >= 0);
        return at(p_m, p_n);
    }
    inline Precision operator()(int p_m, int p_n) const {
        ASSERT(p_m < m && p_n < n && m >= 0 && n >= 0);
        return get(p_m, p_n);
    }

    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {

        using std::scientific;
        using std::defaultfloat;
        using std::fixed;
        using namespace std;

        os << std::fixed << std::setprecision(4) << defaultfloat << std::setfill(' ');

        for (int i = 0; i < matrix.getM(); i++) {
            for (int j = 0; j < matrix.getN(); j++) {
                if(abs(matrix.get(i,j)) < 1e-12){
                    os << std::setw(10) << 0 << " ";
                }else{
                    os << std::setw(10) << matrix.get(i, j) << " ";

                }
            }
            os << "\n";
        }
        return os;
    }

};

#endif //MATLIB_MATRIX_H
