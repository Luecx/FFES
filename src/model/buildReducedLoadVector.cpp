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
#include "Model.h"
#include "../system/LoadCase.h"

Eigen::VectorXd Model::buildReducedLoadVector(LoadCase* load_case) {
    ID           max_id = this->numerateUnconstrainedNodes(load_case);

    Eigen::VectorXd  nodal_forces {max_id, 1};

    ASSERT(           node_data[USED                               ].isInitialised());
    ASSERT(load_case->node_data[BOUNDARY_FORCE                     ].isInitialised());
    ASSERT(           node_data[REDUCED_STIFFNESS_INDEX            ].isInitialised());
    ASSERT(           node_data[BOUNDARY_IMPLIED_DISPLACEMENT_FORCE].isInitialised());

    // first apply nodal forces
    for(int i = 0; i < max_node_count; i++){

        // dont create a load vector for unused nodes
        if(node_data[USED][i][0] == 0) continue;

        // dont create a load vector for nodes not connected to any element
        if(node_data[NODE_CONNECTED_ELEMENTS][i][0] == 0) continue;

        // go over each dimension
        for(int j = 0; j < nodal_dimension; j++){

            // get the dof id
            int dof_id1 = node_data[REDUCED_STIFFNESS_INDEX][i][j];

            // check if its constrained or not
            if (dof_id1 < 0) continue;

            // compute the nodal force
            nodal_forces(dof_id1) = load_case->node_data[BOUNDARY_FORCE][i][j]
                                  + node_data[BOUNDARY_IMPLIED_DISPLACEMENT_FORCE][i][j];
        }
    }

    return nodal_forces;
}