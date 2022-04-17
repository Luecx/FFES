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

#ifndef TOPO_PY_SRC_DELAUNEY_NODE_H_
#define TOPO_PY_SRC_DELAUNEY_NODE_H_
#include <ostream>
#include "../core/defs.h"

namespace delaunay{

struct Node {
    Precision x;
    Precision y;

    bool operator==(const Node& rhs) const;
    bool operator!=(const Node& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const Node& node);
};

}

#endif    // TOPO_PY_SRC_DELAUNEY_NODE_H_
