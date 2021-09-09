//
// Created by Luecx on 07.09.2021.
//
#include "Model.h"
DenseMatrix Model::buildReducedLoadVector(const ListedMatrix& reducedStiffness) {
    ID           max_id = this->numerateUnconstrainedNodes();

    DenseMatrix nodal_forces {max_id, 1};

    ASSERT(node_data[BOUNDARY_FORCE         ].isInitialised());
    ASSERT(node_data[REDUCED_STIFFNESS_INDEX].isInitialised());

    // first apply nodal forces
    for(int i = 0; i < node_count; i++){
        for(int j = 0; j < nodal_dimension; j++){
            int dof_id1 = node_data[REDUCED_STIFFNESS_INDEX][i][j];

            if (dof_id1 < 0) continue;

            nodal_forces(dof_id1, 0) = node_data[BOUNDARY_FORCE][i][j];
        }
    }


    // apply nodal forces due to forced displacement
    // iterate over constrained dofs
    // TODO
//    for(int i = 0)

//    for (auto h : elements) {
//        auto mat        = h->computeLocalStiffness();
//        auto el_n_count = h->nodeCount();
//        auto node_ids   = h->nodeIDS();
//
//        auto n_dof      = mat.m / el_n_count;
//
//        for (int idx = 0; idx < el_n_count; idx++) {
//            for (int idy = 0; idy < el_n_count; idy++) {
//                auto node_id1 = node_ids[idx];
//                auto node_id2 = node_ids[idy];
//
//                for (int dim1 = 0; dim1 < n_dof; dim1++) {
//                    for (int dim2 = 0; dim2 < n_dof; dim2++) {
//
//                        int dof_id1 = node_data[REDUCED_STIFFNESS_INDEX][node_id1][dim1];
//                        int dof_id2 = node_data[REDUCED_STIFFNESS_INDEX][node_id2][dim2];
//                        if (dof_id1 < 0 || dof_id2 < 0) {
//                            continue;
//                        }
//
//                        listed_matrix(dof_id1, dof_id2) += mat(idx + dim1, idy + dim2);
//                    }
//                }
//            }
//        }
//    }
    return nodal_forces;
}