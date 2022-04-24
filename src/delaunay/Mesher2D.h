

//
// Created by Luecx on 21.04.2022.
//

#ifndef TOPO_PY_SRC_DELAUNAY_MESHER2D_H_
#define TOPO_PY_SRC_DELAUNAY_MESHER2D_H_

#include "Loop.h"
#include <vector>
namespace delaunay{
struct Mesher2D {
public:

    std::vector<Loop> loops;

    Mesher2D(const std::vector<Loop> &loops);

    Precision densityFunction(Node node);

};
}

#endif    // TOPO_PY_SRC_DELAUNAY_MESHER2D_H_
