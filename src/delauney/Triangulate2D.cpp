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
#include "Triangulate2D.h"
delaunay::Triangulate2D::Triangulate2D(Precision x_min,
                                       Precision x_max,
                                       Precision y_min,
                                       Precision y_max,
                                       int nodes) {
    // reserve data for the triangles first
    // technically the upper bound is incredibly large since every flip adds two new triangles
    // we use an estimate of nodes * 8 which would imply 5 flips on average.
    // The +1 is required to store the initial triangle
    all_triangles.reserve(nodes * 13 + 1);
    all_nodes    .reserve(nodes + 3);

    // add the first 3 nodes which span the area and mark the first triangle
    all_nodes.push_back(Node{x_min - 1, y_min - 1});
    all_nodes.push_back(Node{x_max + 3, y_min - 1});
    all_nodes.push_back(Node{x_min - 1, y_max + 3});

    // create the very first triangle
    all_triangles.emplace_back();
    all_triangles[0].createEdges(&all_nodes[0], &all_nodes[1], &all_nodes[2]);
}

delaunay::Triangle* delaunay::Triangulate2D::findTriangle(Triangle* triangle, delaunay::Node* node) {
    bool contains = triangle->contains(node);
    // return null if not contained
    if(!contains) return nullptr;
    // if there is no child, use that triangle
    if(triangle->historyDegree == 0){
        return triangle;
    }
    // else go through the other triangles and check them
    for(int i = 0; i< triangle->historyDegree; i++){
        delaunay::Triangle* tri = findTriangle(triangle->history[i], node);
        if(tri != nullptr) return tri;
    }

    return nullptr;
}

delaunay::Triangle* delaunay::Triangulate2D::newTriangle() {
    if(all_triangles.size() >= all_triangles.capacity() - 5) {
        std::cout << "Memory problems when meshing the geometry" << std::endl;
        exit(-1);
    }
    all_triangles.emplace_back();
    return &all_triangles.back();
}

void delaunay::Triangulate2D::addNode(delaunay::Node node) {
    this->all_nodes.push_back(node);
    // get the correct triangle
    delaunay::Triangle* tri = findTriangle(&all_triangles[0], &this->all_nodes.back());
    // exit if the triangle could not be found
    if(tri == nullptr){
        std::cout << "did not find a triangle" << std::endl;
        exit(-1);
    }

    Triangle* t1 = newTriangle();
    Triangle* t2 = newTriangle();
    Triangle* t3 = newTriangle();
    // add three new triangles
    tri->splitIntoThreeTriangles(*t1,
                                 *t2,
                                 *t3,
                                 &this->all_nodes.back());
    // flip vertices
    int age = all_triangles.size();
    t1->getE1().flipIfRequired(age, *this);
    t2->getE1().flipIfRequired(age, *this);
    t3->getE1().flipIfRequired(age, *this);

}
std::ostream& delaunay::operator<<(std::ostream& os, const delaunay::Triangulate2D& d) {
    for(const Node &n:d.all_nodes){
        std::cout << "Node " << &n << " " << n << std::endl;
    }
    int idx = 1;
    for(const Triangle& t:d.all_triangles){
        if(t.historyDegree == 0){
            std::cout << "Triangle " << (idx ++)  << " "
                      << t.getN1() << ";"
                      << t.getN2() << ";"
                      << t.getN3() << std::endl;
        }
    }
    return os;
}
