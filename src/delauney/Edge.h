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

#ifndef TOPO_PY_SRC_DELAUNEY_EDGE_H_
#define TOPO_PY_SRC_DELAUNEY_EDGE_H_

#include <valarray>
#include <ostream>
#include "Node.h"

namespace delaunay{

struct Triangle;
struct Triangulate2D;

struct Edge {
    Node* n1;
    Node* n2;
    Edge* other = nullptr;
    Triangle* triangle;
    int flip_age=0;
    int edge_index=0;

    bool isLeft(Node* n) const;
    bool isDelaunay() const;
    void link(Edge* other);
    void setAge(int age);
    void flipIfRequired(int age,Triangulate2D &triangulate_2d);

    bool operator==(const Edge& rhs) const;
    bool operator!=(const Edge& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const Edge& edge);
};

}

#endif    // TOPO_PY_SRC_DELAUNEY_EDGE_H_
