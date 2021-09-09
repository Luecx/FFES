//
// Created by finne on 7/29/2020.
//

#include "ListedMatrix.h"

#include <algorithm>
#include <cassert>

ListedMatrix::ListedMatrix(int mP, int nP) : Matrix(mP, nP) {
    rows = new std::vector<RowEntry>[mP]{};
}

ListedMatrix::ListedMatrix(const ListedMatrix& other) : Matrix(other) {
    rows = new std::vector<RowEntry>[other.getM()];
    for (int i = 0; i < other.getM(); i++) {
        rows[i] = other.getRow(i);
    }
}

ListedMatrix::ListedMatrix(ListedMatrix&& other) : Matrix(other) {
    rows = other.rows;
    other.rows = nullptr;
}

ListedMatrix::~ListedMatrix() {
    if(rows == nullptr){
        return;
    }
    delete[] rows;
    rows = nullptr;
}

std::vector<RowEntry> ListedMatrix::getRow(int row) const { return rows[row]; }

uint64_t                   ListedMatrix::mem() {

    int t = 0;

    for (int i = 0; i < getM(); i++) {
        t += sizeof(rows[i]);
        t += sizeof(RowEntry) * rows[i].size();
    }

    return t + sizeof(int) * 2;
}

void ListedMatrix::reset() {
    for (int i = 0; i < getM(); i++) {
        rows[i].clear();
    }
}
void ListedMatrix::set(int p_m, int p_n, Precision val) {
    for (int i = 0; i < rows[p_m].size(); i++) {
        if (rows[p_m].at(i).column == p_n) {
            rows[p_m].at(i).value = val;
        }
    }

    rows[p_m].emplace_back(p_n, val);
}
Precision& ListedMatrix::at(int p_m, int p_n) {
    static int counter = 0;

    for (int i = 0; i < rows[p_m].size(); i++) {
        if (rows[p_m].at(i).column == p_n) {
            return rows[p_m].at(i).value;
        }
    }

    counter++;
    rows[p_m].emplace_back(RowEntry(p_n, 0));

    return rows[p_m].back().value;
}
Precision ListedMatrix::get(int p_m, int p_n) const {
    for (int i = 0; i < rows[p_m].size(); i++) {
        if (rows[p_m].at(i).column == p_n) {
            return rows[p_m].at(i).value;
        }
    }
    return 0;
}

//void ListedMatrix::set(int p_m, int p_n, Precision val) {
//
//    for (int i = 0; i < rows[p_m].size(); i++) {
//        if (rows[p_m].at(i).column == p_n) {
//            rows[p_m].at(i).value = val;
//        }
//    }
//
//    rows[p_m].emplace_back(p_n, val);
//}
// double &ListedMatrix::at(int m, int n) {
//     static int counter = 0;
//
//     for(int i = 0; i < rows[m].size(); i++){
//         if(rows[m].at(i).column == n) {
//             return rows[m].at(i).value;
//         }
//     }
//
//     counter ++;
//     rows[m].emplace_back(std::move(RowEntry(n,0)));
//
//     return rows[m].back().value;
// }
//
// double ListedMatrix::get(int m, int n) const {
//
//     for(int i = 0; i < rows[m].size(); i++){
//         if(rows[m].at(i).column == n) {
//             return rows[m].at(i).value;
//         }
//     }
//     return 0;
// }

RowEntry::RowEntry(int column, Precision value) : column(column), value(value) {}

bool RowEntry::operator<(const RowEntry& rhs) const { return column < rhs.column; }

bool RowEntry::operator>(const RowEntry& rhs) const { return rhs < *this; }

bool RowEntry::operator<=(const RowEntry& rhs) const { return !(rhs < *this); }

bool RowEntry::operator>=(const RowEntry& rhs) const { return !(*this < rhs); }

std::ostream&  operator<<(std::ostream& os, const RowEntry& entry) {
    os << "column: " << entry.column << " value: " << entry.value;
    return os;
}
