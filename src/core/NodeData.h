/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFESis free software: you can redistribute it and/or modify                   *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFESis distributed in the hope that it will be useful,                     *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

#ifndef FEM_SRC_CORE_NODEDATA_H_
#define FEM_SRC_CORE_NODEDATA_H_

#include "ComponentContainer.h"
#include "defs.h"

#include <ostream>

enum NodeDataEntries{
    USED,                                   // check if used by any element
    POSITION,                               // coordinates

    // LOAD CASE DATA
    BOUNDARY_IS_CONSTRAINED_NEXT_CASE,      // contains information about the bc for the next case
    BOUNDARY_IS_CONSTRAINED,                // 1 if boundary displacement is constrained (see below)
    BOUNDARY_DISPLACEMENT_NEXT_CASE,        // contains information about the bc for the next case
    BOUNDARY_DISPLACEMENT,                  // boundary displacement, relevant if BOUNDARY_IS_CONSTRAINED = 1
    BOUNDARY_FORCE_NEXT_CASE,               // contains information about the bc for the next case
    BOUNDARY_FORCE,                         // boundary force, not relevant if constrained

    BOUNDARY_IMPLIED_DISPLACEMENT_FORCE,    // force implied due to displacements
    DISPLACEMENT,                           // displacement in final solution
    STRESS,
    N_MAX_NODE_FLOAT_ENTRIES
};

enum NodeDataIntegerEntries{
    REDUCED_STIFFNESS_INDEX,
    NODE_CONNECTED_ELEMENTS,
    N_MAX_NODE_INT_ENTRIES
};

struct NodeData{
    ComponentContainer<Precision> float_data[N_MAX_NODE_FLOAT_ENTRIES]{};
    ComponentContainer<int>       int_data  [N_MAX_NODE_INT_ENTRIES]{};

    ComponentContainer<Precision>& operator[](NodeDataEntries entry){
        return float_data[entry];
    }

    ComponentContainer<int>&       operator[](NodeDataIntegerEntries entry){
        return int_data[entry];
    }

};

#endif    // FEM_SRC_CORE_NODEDATA_H_
