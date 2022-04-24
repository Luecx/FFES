

//
// Created by Luecx on 16.04.2022.
//

#include "Loop.h"

#include <utility>
delaunay::Loop::Loop(std::vector<Border>  borders) : borders(std::move(borders)){
}
void delaunay::Loop::exclude() {
    includes_content = false;
}
void delaunay::Loop::include() {
    includes_content = true;
}
bool delaunay::Loop::includes(delaunay::Node* node) const{
    return isInside(node) ^ !includes_content;
}
bool delaunay::Loop::isInside(delaunay::Node* node) const{
    int crosses = 0;
    for(const Border &e:borders){

        Coordinate x_upper = e.n1.x;
        Coordinate x_lower = e.n2.x;
        Coordinate y_upper = e.n1.y;
        Coordinate y_lower = e.n2.y;

        // get the intersection with the horizontal ray of the node
        Coordinate y_node = node->y;

        if(y_upper < y_lower){
            std::swap(x_upper, x_lower);
            std::swap(y_upper, y_lower);
        }

        // dont consider horizontal lines
        if(y_upper == y_lower) continue;

        // dont consider if the y coordinate of the node is not contained
        if(y_upper <= y_node || y_lower > y_node) continue;

        Precision x_intersection =
            static_cast<Precision>(x_lower) +
            static_cast<Precision>(x_upper - x_lower) /
            static_cast<Precision>(y_upper - y_lower) *
            static_cast<Precision>(y_node  - y_lower);

        if(x_intersection > static_cast<Precision>(node->x)){
            crosses ++;
        }
    }
    return crosses % 2 == 1;
}
