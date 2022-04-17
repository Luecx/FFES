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

#ifndef TOPO_PY_SRC_DELAUNEY_LOOP_H_
#define TOPO_PY_SRC_DELAUNEY_LOOP_H_

#include <vector>
#include "Node.h"
#include "Edge.h"
#include "Triangle.h"

namespace delaunay{

struct Loop {
    std::vector<Node*> nodes{};
    std::vector<Edge> edges{};
    bool includes_content=true;

    public:
    Loop(const std::vector<Node*>& nodes);

    void excludes();
    void includes();
    bool isIncluded(Triangle &triangle);
    bool isInside(Node* node);
};



}

#endif    // TOPO_PY_SRC_DELAUNEY_LOOP_H_
