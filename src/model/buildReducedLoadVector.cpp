
#include "Model.h"
#include "../system/LoadCase.h"

Eigen::Matrix<Precision,Eigen::Dynamic,1> Model::buildReducedLoadVector(LoadCase* load_case) {
    ID           max_id = this->numerateUnconstrainedNodes(load_case);

    Eigen::Matrix<Precision,Eigen::Dynamic,1>  nodal_forces {max_id, 1};

    ERROR(load_case->node_data[BOUNDARY_FORCE                     ].isInitialised(), UNINITIALISED, "data is not initialised");
    ERROR(           node_data[REDUCED_STIFFNESS_INDEX            ].isInitialised(), UNINITIALISED, "data is not initialised");
    ERROR(           node_data[BOUNDARY_IMPLIED_DISPLACEMENT_FORCE].isInitialised(), UNINITIALISED, "data is not initialised");

    // first apply nodal forces
    for(int i = 0; i < max_node_count; i++){

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