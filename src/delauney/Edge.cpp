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
// Created by Luecx on 13.04.2022.
//

#include <iostream>
#include "Triangle.h"
#include "Edge.h"
#include "Triangulate2D.h"

bool delaunay::Edge::isLeft(delaunay::Node* n) const {
    auto h = (  (n2->x - n1->x)*(n->y - n1->y)
              - (n2->y - n1->y)*(n->x - n1->x));
    return h >= 0;
}
bool delaunay::Edge::isDelaunay() const{
    if(other){
        return !triangle->imscribed(other->triangle->opposite(other));
    }
    return true;
}
void delaunay::Edge::link(delaunay::Edge* other){
    this->other = other;
    if(other) {
        other->other = this;
        this->flip_age = other->flip_age = std::max(this->flip_age, other->flip_age);
    }
}
void delaunay::Edge::setAge(int age) {
    this->flip_age = age;
    if(other)
        other->flip_age = age;
}
void delaunay::Edge::flipIfRequired(int age,Triangulate2D &triangulate_2d) {
    if(other == nullptr) return;
    if(triangle == nullptr) return;
    if(other->triangle == nullptr) return;
    // check if the edge has already been flipped
    if(flip_age == age) return;
    // check if the other node is within the imscribed triangle
    Node* other_node = other->triangle->opposite(other);
    if(triangle->imscribed(other_node)){
        // require flipping
        Node* our_node = triangle->opposite(this);
        // create two new triangles
        Triangle* t1 = triangulate_2d.newTriangle();
        Triangle* t2 = triangulate_2d.newTriangle();
        t1->createEdges(our_node, other_node, n2);
        t2->createEdges(other_node, our_node, n1);
        // link neighbours
        Edge* other_edge_next = &other->triangle->getEdge((this->other->edge_index + 1) % 3);
        Edge* other_edge_prev = &other->triangle->getEdge((this->other->edge_index + 2) % 3);
        Edge*       edge_next = &       triangle->getEdge((             edge_index + 1) % 3);
        Edge*       edge_prev = &       triangle->getEdge((             edge_index + 2) % 3);

        t1->e1.link(&t2->e1);
        t1->e2.link(other_edge_prev->other);
        t1->e3.link(      edge_next->other);
        t2->e2.link(      edge_prev->other);
        t2->e3.link(other_edge_next->other);
        // set histories
        triangle->historyDegree = 2;
        triangle->history[0] = t1;
        triangle->history[1] = t2;
        other->triangle->historyDegree = 2;
        other->triangle->history[0] = t1;
        other->triangle->history[1] = t2;
        // set age
        flip_age = age;
        other->flip_age = age;
        t1->e1.flip_age = age;
        t2->e1.flip_age = age;
        // continue flipping
        t1->getEdge(1). flipIfRequired(age, triangulate_2d);
        t2->getEdge(2). flipIfRequired(age, triangulate_2d);
    }else{
        flip_age = age;
        other->flip_age = age;
    }
}
bool delaunay::Edge::operator==(const delaunay::Edge& rhs) const { return *n1 == *rhs.n1 && *n2 == *rhs.n2; }
bool delaunay::Edge::operator!=(const delaunay::Edge& rhs) const { return !(rhs == *this); }
std::ostream& delaunay::operator<<(std::ostream& os, const delaunay::Edge& edge) {
    os << "n1: " << *edge.n1
       << " n2: " << *edge.n2 << " other: " << edge.other
       << " triangle: " << edge.triangle << " flip_age: " << edge.flip_age
       << " edge_index: " << edge.edge_index;
    return os;
}
