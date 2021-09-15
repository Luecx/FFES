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
ID Model::numerateUnconstrainedNodes() {

    /**
     * BOUNDARY_IS_CONSTRAINED,        // 1 if boundary displacement is constrained (see below)
     * BOUNDARY_DISPLACEMENT,          // boundary displacement, relevant if BOUNDARY_IS_CONSTRAINED = 1
     *
     * REDUCED_STIFFNESS_INDEX,
     */

    // make sure to reallocate if needed
    node_data[REDUCED_STIFFNESS_INDEX  ]
        .init(max_node_count * nodal_dimension, max_node_count)
        .even(nodal_dimension);

    // incrementally count IDS
    int ids = 0;

    ASSERT(this->node_data[USED                   ].isInitialised());
    ASSERT(this->node_data[BOUNDARY_IS_CONSTRAINED].isInitialised());
    ASSERT(this->node_data[BOUNDARY_DISPLACEMENT  ].isInitialised());

    // check each node and if its constrained, if so remove those indices
    for(int i = 0; i < max_node_count; i++){
        // dont numerate if its not used
        if(node_data[USED][i][0] == 0) continue;

        // go over each dimension of the node
        for(int j = 0; j < nodal_dimension; j++){

            // if its not constrained, assign an id
            if(this->node_data[BOUNDARY_IS_CONSTRAINED][i][j] == 0){
                this->node_data[REDUCED_STIFFNESS_INDEX][i][j] = ids++;
            }else{
                this->node_data[REDUCED_STIFFNESS_INDEX][i][j] = -1;
            }
        }
    }
    return ids;
}