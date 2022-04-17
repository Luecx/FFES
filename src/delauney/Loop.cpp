/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFES is free software: you can redistribute it and/or modify                  *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFES is distributed in the hope that it will be useful,                    *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

//
// Created by Luecx on 16.04.2022.
//

#include "Loop.h"
delaunay::Loop::Loop(const std::vector<Node*>& nodes) : nodes(nodes) {
    for(int i = 0; i < nodes.size(); i++){
        this->edges.push_back(Edge{nodes[i], nodes[(i+1)%nodes.size()]});
    }
}
void delaunay::Loop::excludes() {
    includes_content = false;
}
void delaunay::Loop::includes() {
    includes_content = true;
}
bool delaunay::Loop::isIncluded(delaunay::Triangle& triangle) {
    Precision center_x = (triangle.getN1()->x +
        triangle.getN2()->x +
        triangle.getN3()->x) / 3.0;
    Precision center_y = (triangle.getN1()->y +
        triangle.getN2()->y +
        triangle.getN3()->y) / 3.0;
    Node n{center_x, center_y};
    return isInside(&n) ^ !includes_content;
}
bool delaunay::Loop::isInside(delaunay::Node* node) {
    int crosses = 0;
    for(const Edge &e:edges){

        Precision x_upper = e.n1->x;
        Precision x_lower = e.n2->x;
        Precision y_upper = e.n1->y;
        Precision y_lower = e.n2->y;

        // get the intersection with the horizontal ray of the node
        Precision y_node = node->y;

        if(y_upper < y_lower){
            std::swap(x_upper, x_lower);
            std::swap(y_upper, y_lower);
        }

        // dont consider horizontal lines
        if(y_upper == y_lower) continue;

        // dont consider if the y coordinate of the node is not contained
        if(y_upper <= y_node || y_lower > y_node) continue;

        Precision x_intersection = x_lower + (x_upper - x_lower) / (y_upper - y_lower) * (y_node - y_lower);

        if(x_intersection > node->x){
            crosses ++;
        }
    }
    return crosses % 2 == 1;
}
