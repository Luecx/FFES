//
// Created by finne on 7/29/2020.
//

#ifndef MATLIB_LISTEDMATRIX_H
#define MATLIB_LISTEDMATRIX_H


#include <vector>
#include <ostream>
#include "Matrix.h"

struct RowEntry {
    int    column = 0;
    Precision value  = 0;
    
    bool operator<(const RowEntry &rhs) const;
    
    bool operator>(const RowEntry &rhs) const;
    
    bool operator<=(const RowEntry &rhs) const;
    
    bool operator>=(const RowEntry &rhs) const;
    
    RowEntry(int column, Precision value);
    
    RowEntry() {}
    
    
    RowEntry(const RowEntry &rowEntry) {
        this->column = rowEntry.column;
        this->value  = rowEntry.value;
    }
    
    
    RowEntry(const RowEntry &&rowEntry) {
        this->column = rowEntry.column;
        this->value  = rowEntry.value;
    }
    
    RowEntry &operator=(const RowEntry &rhs) {
        this->value  = rhs.value;
        this->column = rhs.column;
        return *this;
    }
    
    RowEntry &operator=(RowEntry &&rhs) noexcept // move assignment
    {
        this->value  = rhs.value;
        this->column = rhs.column;
        return *this;
    }
    
    friend std::ostream &operator<<(std::ostream &os, const RowEntry &entry);
};

class ListedMatrix : public Matrix {
        std::vector<RowEntry> *rows;
    public:
        ListedMatrix(int mP, int nP);
        
        ListedMatrix(const ListedMatrix &other);

        ListedMatrix( ListedMatrix &&other);
        
        ListedMatrix() = delete;
        
        virtual ~ListedMatrix();
        
        std::vector<RowEntry> getRow(int row) const;
        
        uint64_t mem() override;
        
        void reset() override;

        void                   set(int p_m, int p_n, Precision val) override;
        Precision&             at(int p_m, int p_n) override;
        Precision              get(int p_m, int p_n) const override;
};


#endif //MATLIB_LISTEDMATRIX_H
