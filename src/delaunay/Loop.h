

//
// Created by Luecx on 16.04.2022.
//

#ifndef TOPO_PY_SRC_DELAUNEY_LOOP_H_
#define TOPO_PY_SRC_DELAUNEY_LOOP_H_

#include <vector>
#include "Node.h"
#include "Edge.h"
#include "Triangle.h"
#include "Border.h"

namespace delaunay{

struct Loop {
    std::vector<Border> borders{};
    bool includes_content=true;

    public:
    Loop(std::vector<Border>  borders);

    void exclude();
    void include();
    bool includes(delaunay::Node* node) const;
    bool isInside(Node* node) const;
};



}

#endif    // TOPO_PY_SRC_DELAUNEY_LOOP_H_
