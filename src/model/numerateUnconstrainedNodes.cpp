//
// Created by Luecx on 07.09.2021.
//
#include "Model.h"
ID Model::numerateUnconstrainedNodes() {

    /**
     * BOUNDARY_IS_CONSTRAINED,        // 1 if boundary displacement is constrained (see below)
     * BOUNDARY_DISPLACEMENT,          // boundary displacement, relevant if BOUNDARY_IS_CONSTRAINED = 1
     *
     * REDUCED_STIFFNESS_INDEX,
     */

    // make sure to reallocate if needed
    node_data[REDUCED_STIFFNESS_INDEX  ].init(node_count * 3, node_count);

    // incrementally count IDS
    int ids = 0;

    ASSERT(this->node_data[BOUNDARY_IS_CONSTRAINED].isInitialised());
    ASSERT(this->node_data[BOUNDARY_DISPLACEMENT  ].isInitialised());

    // check each node and if its constrained, if so remove those indices
    for(int i = 0; i < node_count; i++){
        node_data[REDUCED_STIFFNESS_INDEX].setIndexIncremental(i, nodal_dimension);
        for(int j = 0; j < nodal_dimension; j++){
            if(this->node_data[BOUNDARY_IS_CONSTRAINED][i][j] == 0){
                this->node_data[REDUCED_STIFFNESS_INDEX][i][j] = ids++;
            }else{
                this->node_data[REDUCED_STIFFNESS_INDEX][i][j] = -1;
            }
        }
    }
    return ids;
}