
//
// Created by Luecx on 23.04.2022.
//

#ifndef EXACTCONSTRAINEDDELAUNAY_SRC_DEFS_H_
#define EXACTCONSTRAINEDDELAUNAY_SRC_DEFS_H_

#include <cstdint>
#include <iostream>

namespace delaunay{

struct Node;
struct Edge;
struct Triangle;
struct CDT;

using Precision      = double;
using HalfCoordinate = int32_t;
using Coordinate     = int64_t;
using Index          = int32_t;
using Age            = uint32_t;

enum Coordinates : HalfCoordinate{
    MIN_HARD_COORDINATE = - (1 << 29),
    MAX_HARD_COORDINATE = + (1 << 29),
    MIN_SOFT_COORDINATE = - (1 << 27),
    MAX_SOFT_COORDINATE = + (1 << 27)
};

inline Index nextIndex(Index idx){
    return (idx + 1) % 3;
}
inline Index prevIndex(Index idx){
    return (idx + 2) % 3;
}
inline HalfCoordinate toCoordinate(Precision value, Precision min, Precision max){
    return MIN_SOFT_COORDINATE + static_cast<HalfCoordinate>((MAX_SOFT_COORDINATE - MIN_SOFT_COORDINATE) * ((value - min) / (max - min)));
}

}

#endif //EXACTCONSTRAINEDDELAUNAY_SRC_DEFS_H_
