
//
// Created by Luecx on 24.04.2022.
//

#ifndef EXACTCONSTRAINEDDELAUNAY_SRC_BORDER_H_
#define EXACTCONSTRAINEDDELAUNAY_SRC_BORDER_H_

#include <vector>
#include "Node.h"
namespace delaunay{

struct Border{
    Node n1;
    Node n2;
    std::vector<Node> nodes{};
    int divisions = 1;

    Border(Node n_1, Node n_2, int divisions);

};
}

#endif //EXACTCONSTRAINEDDELAUNAY_SRC_BORDER_H_
