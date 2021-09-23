
#include "Model.h"
#include "../system/LoadCase.h"

void Model::postProcessDisplacements(const Eigen::Matrix<Precision,Eigen::Dynamic,1>& displacement,
                                     LoadCase* load_case) {

    load_case->node_data[DISPLACEMENT].init(max_node_count * nodal_dimension, max_node_count)
        .even(nodal_dimension);

    for(int i = 0; i < max_node_count; i++){

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
