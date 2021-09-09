//
// Created by Luecx on 07.09.2021.
//
#include "Model.h"
ListedMatrix Model::buildReducedStiffnessMatrix() {
    ID           max_id = this->numerateUnconstrainedNodes();

    ListedMatrix listed_matrix {max_id, max_id};

    for (auto h : elements) {
        auto mat        = h->computeLocalStiffness();
        auto el_n_count = h->nodeCount();
        auto node_ids   = h->nodeIDS();

        auto n_dof      = mat.getM() / el_n_count;



        for (int idx = 0; idx < el_n_count; idx++) {
            for (int idy = 0; idy < el_n_count; idy++) {
                auto node_id1 = node_ids[idx];
                auto node_id2 = node_ids[idy];

                for (int dim1 = 0; dim1 < n_dof; dim1++) {
                    for (int dim2 = 0; dim2 < n_dof; dim2++) {

                        int dof_id1 = node_data[REDUCED_STIFFNESS_INDEX][node_id1][dim1];
                        int dof_id2 = node_data[REDUCED_STIFFNESS_INDEX][node_id2][dim2];
                        if (dof_id1 < 0 || dof_id2 < 0) {
                            continue;
                        }
                        listed_matrix(dof_id1, dof_id2) += mat(idx * n_dof + dim1, idy * n_dof + dim2);
                    }
                }
            }
        }
    }
    return listed_matrix;
}