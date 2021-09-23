

#ifndef FEM_SRC_MATRIX_DENSEMATRIX_H_
#define FEM_SRC_MATRIX_DENSEMATRIX_H_

#include "Matrix.h"
#include "QuickMatrix.h"
#include <cstring>

class DenseMatrix : public Matrix {
    protected:
    Precision* data = nullptr;

    public:
    DenseMatrix(int p_m, int p_n) : Matrix(p_m, p_n) { this->data = new Precision[p_m * p_n] {}; }
    template<typename... Args>
    DenseMatrix(int p_m, int p_n, Args... values) : Matrix(p_m, p_n) { this->data = new Precision[p_m * p_n] {values...}; }
    DenseMatrix(const DenseMatrix& other) : Matrix(other.m, other.n) {
        this->data = new Precision[m * n] {};
        std::memcpy(data, other.data, sizeof(Precision) * m * n);
    }
    DenseMatrix(DenseMatrix&& other) noexcept : Matrix(other.m, other.n) {
        this->data = other.data;
        other.data = nullptr;
    }
    template<int M, int N> DenseMatrix(const QuickMatrix<M,N>& other) : Matrix(M,N) {
        this->data = new Precision[m * n]{};
        for(int i = 0; i < M; i++){
            for(int p_n = 0; p_n < N; p_n++){
                this->at(i,p_n) = other(i,p_n);
            }
        }
    }
    virtual ~DenseMatrix() {
        if(data != nullptr){
            delete[] data;
            data = nullptr;
        }
    }

    uint64_t mem() override {
        return sizeof(data) * this->m * this->n;
    }
    void reset() override {
        std::memset(data, 0, sizeof(data) * m * n);
    }
    void set(int p_m, int p_n, Precision val) override {
        ASSERT(p_m < m &&
               p_n < n &&
               m >= 0 &&
               n >= 0);
        data[p_m*this->n + p_n] = val;
    }
    Precision &at(int p_m, int p_n) override {
        ASSERT(p_m < m &&
               p_n < n &&
               m >= 0 &&
               n >= 0);
        return data[p_m * n + p_n];
    }
    [[nodiscard]] Precision get(int p_m, int p_n) const override {
        ASSERT(p_m < m &&
               p_n < n &&
               m >= 0 &&
               n >= 0);
        return data[p_m * this->n + p_n];
    }
    void swapRows(int m1, int m2){
        ASSERT(m1 < m &&
               m2 < m &&
               m1 >= 0 &&
               m2 >= 0);
        for(int i = 0; i < n;i ++){
            std::swap(at(m1,i),at(m2,i));
        }
    }
    void identity(Precision v = 1){
        (*this) = 0;
        for(int i = 0; i < std::min(this->m, this->n); i++){
            at(i,i) = v;
        }
    }
    void randomise() {
        for(int i = 0; i < this->m; i++){
            for(int p_n = 0; p_n < this->n; p_n++){
                at(i,p_n) = (rand() / static_cast<float>(RAND_MAX));
            }
        }
    }
    Precision dot(const DenseMatrix& other) const{
        ASSERT(m == other.m);
        ASSERT(n == other.n);
        Precision res = 0;
        for(int i = 0; i < m * n; i++){
            res += data[i] * other.data[i];
        }
        return res;
    }
    DenseMatrix hadamard(const DenseMatrix& other) const{
        ASSERT(m == other.m);
        ASSERT(n == other.n);
        DenseMatrix res{*this};
        for(int i = 0; i < m * n; i++){
            res.data[i] = data[i] * other.data[i];
        }
        return res;
    }

    DenseMatrix& operator=(const DenseMatrix& other){
        ASSERT(m == other.m &&
               n == other.n);
        std::memcpy(data, other.data, sizeof(Precision) * m * n);
        return *this;
    }
    DenseMatrix& operator=(DenseMatrix&& other){
        delete[] this->data;
        this->data = other.data;
        other.data = nullptr;
        this->m = other.m;
        this->n = other.n;
        return *this;
    }
    DenseMatrix& operator=(const Precision s){
        std::memset(data, s, sizeof(Precision) * m * n);
        return *this;
    }

    DenseMatrix& operator+=(const DenseMatrix& other){
        ASSERT(m == other.m &&
               n == other.n);
        for(int p_m = 0; p_m < this->m; p_m++){
            for(int p_n = 0; p_n < this->n; p_n++){
                this->at(p_m,p_n) += other(p_m,p_n);
            }
        }
        return *this;
    }
    DenseMatrix operator+(const DenseMatrix& other) const{
        ASSERT(m == other.m &&
               n == other.n);
        DenseMatrix res{*this};
        res += other;
        return res;
    }
    DenseMatrix& operator-=(const DenseMatrix& other){
        ASSERT(m == other.m &&
               n == other.n);
        for(int p_m = 0; p_m < this->m; p_m++){
            for(int p_n = 0; p_n < this->n; p_n++){
                this->at(p_m,p_n) -= other(p_m,p_n);
            }
        }
        return *this;
    }
    DenseMatrix operator-(const DenseMatrix& other) const{
        ASSERT(m == other.m &&
               n == other.n);
        DenseMatrix res{*this};
        res -= other;
        return res;
    }
    DenseMatrix operator-() const{
        DenseMatrix res{*this};
        for(int p_m = 0; p_m < this->m; p_m++){
            for(int p_n = 0; p_n < this->n; p_n++){
                res(p_m,p_n) = -get(p_m,p_n);
            }
        }
        return res;
    }
    DenseMatrix operator!() const{
        DenseMatrix res{n,m};
        for(int p_m = 0; p_m < this->m; p_m++){
            for(int p_n = 0; p_n < this->n; p_n++){
                res.at(p_n,p_m) = get(p_m,p_n);
            }
        }
        return res;
    }

    DenseMatrix operator*(const DenseMatrix& other) const{
        ASSERT(n == other.m);
        DenseMatrix res{m, other.n};
        for(int p_m = 0; p_m < this->m; p_m++){
            for(int p_g = 0; p_g < other.n; p_g++){
                Precision sum = 0;
                for(int p_n = 0; p_n < this->n; p_n++){
                    sum += other(p_n,p_g) * get(p_m,p_n);
                }
                res(p_m,p_g) = sum;
            }
        }
        return res;
    }
    DenseMatrix operator*(const Precision s) const{
        DenseMatrix res{*this};
        res *= s;
        return res;
    }
    DenseMatrix& operator*=(const Precision s){
        for(int p_m = 0; p_m < this->m; p_m++){
            for(int p_n = 0; p_n < this->n; p_n++){
                at(p_m,p_n) *= s;
            }
        }
        return *this;
    }

};


#endif //FEM_SRC_MATRIX_DenseMatrix_H_
