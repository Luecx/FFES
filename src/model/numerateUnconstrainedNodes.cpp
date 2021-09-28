
#include "Model.h"
#include "../system/LoadCase.h"

ID Model::numerateUnconstrainedNodes(LoadCase* load_case) {

    // make sure to reallocate if needed
    node_data[REDUCED_STIFFNESS_INDEX]
        .init(max_node_count * nodal_dimension, max_node_count)
        .even(nodal_dimension);

    // inits the amount of connected elements for each element
    this->updateNodeConnectedCount();

    // incrementally count IDS
    int ids = 0;

    ERROR(load_case->node_data[BOUNDARY_IS_CONSTRAINED].isInitialised(), UNINITIALISED, "data is not initialised");

    // check each node and if its constrained, if so remove those indices
    for(int i = 0; i < max_node_count; i++){

        // dont numerate nodes not connected to any element
        if(node_data[NODE_CONNECTED_ELEMENTS][i][0] == 0) continue;

        // go over each dimension of the node
        for(int j = 0; j < nodal_dimension; j++){

            // if its not constrained, assign an id
            if(load_case->node_data[BOUNDARY_IS_CONSTRAINED][i][j] == 0){
                this->node_data[REDUCED_STIFFNESS_INDEX][i][j] = ids++;
            }else{
                this->node_data[REDUCED_STIFFNESS_INDEX][i][j] = -1;
            }
        }
    }
    return ids;
}