//
// Created by Luecx on 29.08.2021.
//

#ifndef FEM_SRC_MATRIX_QUICKMATRIX_H_
#define FEM_SRC_MATRIX_QUICKMATRIX_H_

#include "Matrix.h"
#include <cstring>

template<int M, int N=1>
class QuickMatrix : public Matrix{
protected:
    Precision data[M][N]{};
public:
QuickMatrix() : Matrix(M,N) {}
QuickMatrix(const QuickMatrix<M,N>& other) : Matrix(M,N) {
        std::memcpy(data, other.data, sizeof(data));
    }

    uint64_t mem() override {
        return sizeof(QuickMatrix<M,N>);
    }
    void reset() override {
        std::memset(data, 0, sizeof(data));
    }
    void set(int p_m, int p_n, Precision val) override {
        data[p_m][p_n] = val;
    }
    Precision &at(int p_m, int p_n) override {
        return data[p_m][p_n];
    }
    [[nodiscard]] Precision get(int p_m, int p_n) const override {
        return data[p_m][p_n];
    }
    void swapRows(int m1, int m2){
        for(int i = 0; i < N;i ++){
            std::swap(data[m1][i], data[m2][i]);
        }
    }
    void identity(Precision v = 1){
        (*this) = 0;
        for(int i = 0; i < std::min(M,N); i++){
            at(i,i) = v;
        }
    }
    void randomise() {
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                data[i][j] = (rand() / static_cast<float>(RAND_MAX));
            }
        }
    }

    QuickMatrix<M,N>& operator=(const QuickMatrix<M,N>& other){
        std::memcpy(data, other.data, sizeof(data));
        return *this;
    }
    QuickMatrix<M,N>& operator=(QuickMatrix<M,N>&& other){
        this->data = other.data;
        other.data = nullptr;
        return *this;
    }
    QuickMatrix<M,N>& operator=(const Precision s){
        std::memset(data, s, sizeof(data));
        return *this;
    }

    QuickMatrix<M,N>& operator+=(const QuickMatrix<M,N>& other){
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                this->data[i][j] += other(i,j);
            }
        }
        return *this;
    }
    QuickMatrix<M,N> operator+(const QuickMatrix<M,N>& other){
        QuickMatrix<M,N> res{*this};
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                res(i,j) += other(i,j);
            }
        }
        return res;
    }
    QuickMatrix<M,N>& operator-=(const QuickMatrix<M,N>& other){
        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                this->data[i][j] -= other(i,j);
            }
        }
        return *this;
    }
    QuickMatrix<M,N> operator-(const QuickMatrix<M,N>& other){
        QuickMatrix<M,N> res{*this};
        res -= other;
        return res;
    }
    QuickMatrix<M,N> operator-(){
        QuickMatrix<M,N> res{*this};
        for(int p_m = 0; p_m < M; p_m++){
            for(int p_n = 0; p_n < N; p_n++){
                res(p_m,p_n) -= data[p_m][p_n];
            }
        }
        return res;
    }
    QuickMatrix<N,M> operator!(){
        QuickMatrix<N,M> res{};
        for(int p_m = 0; p_m < M; p_m++){
            for(int p_n = 0; p_n < N; p_n++){
                res(p_n,p_m) = this->data[p_m][p_n];
            }
        }
        return res;
    }


    template<int G> QuickMatrix<M,G> operator*(const QuickMatrix<N,G>& other){
        QuickMatrix<M,G> res{};
        for(int p_m = 0; p_m < M; p_m++){
            for(int p_g = 0; p_g < G; p_g++){
                Precision sum = 0;
                for(int p_n = 0; p_n < N; p_n++){
                    sum += other(p_n,p_g) * data[p_m][p_n];
                }
                res(p_m,p_g) = sum;
            }
        }
        return res;
    }
    QuickMatrix<M,N> operator*(const Precision s){
        QuickMatrix<M,N> res{*this};
        for(int p_m = 0; p_m < M; p_m++){
            for(int p_n = 0; p_n < N; p_n++){
                res(p_m,p_n) *= s;
            }
        }
        return res;
    }
    QuickMatrix<M,N>& operator*=(const Precision s){
        for(int p_m = 0; p_m < M; p_m++){
            for(int p_n = 0; p_n < N; p_n++){
                data[p_m][p_n] *= s;
            }
        }
        return *this;
    }


};


#endif    // FEM_SRC_MATRIX_QUICKMATRIX_H_
