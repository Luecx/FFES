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

void Model::postProcessDisplacements(const Eigen::VectorXd& displacement, LoadCase* load_case) {

    load_case->node_data[DISPLACEMENT].init(max_node_count * nodal_dimension, max_node_count)
        .even(nodal_dimension);

    for(int i = 0; i < max_node_count; i++){

        // dont create a load vector for unused nodes
        if(node_data[USED][i][0] == 0) continue;

        // dont numerate nodes not connected to any element
        if(node_data[NODE_CONNECTED_ELEMENTS][i][0] == 0) continue;

        // iterate over each dimension
        for(int d = 0; d < nodal_dimension; d++){

            // get the dof id
            auto dof_id = node_data[REDUCED_STIFFNESS_INDEX][i][d];

            // check if its constrained
            if(dof_id < 0){
                load_case->node_data[DISPLACEMENT][i][d] = load_case->node_data[BOUNDARY_DISPLACEMENT][i][d];
            }else{
                load_case->node_data[DISPLACEMENT][i][d] = displacement(dof_id);
            }

        }
    }
}
