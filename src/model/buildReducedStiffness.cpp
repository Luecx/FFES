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
Eigen::SparseMatrix<Precision> Model::buildReducedStiffnessMatrix(LoadCase* load_case) {
    ID           max_id = this->numerateUnconstrainedNodes(load_case);

    std::vector<Eigen::Triplet<Precision>> indices{};

    ASSERT(this     ->node_data[REDUCED_STIFFNESS_INDEX            ].isInitialised());
    ASSERT(this     ->node_data[BOUNDARY_IMPLIED_DISPLACEMENT_FORCE].isInitialised());
    ASSERT(load_case->node_data[BOUNDARY_DISPLACEMENT              ].isInitialised());

    for (auto h : elements) {

        // ignore null elements
        if(h == nullptr) continue;

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

                        // correct implied forces due to displacements
                        if (dof_id1 < 0 || dof_id2 < 0) {
                            if(dof_id1 >= 0){
                                this->node_data[BOUNDARY_IMPLIED_DISPLACEMENT_FORCE][node_id1][dim1] -=
                                    mat(idx * n_dof + dim1, idy * n_dof + dim2) *
                                    load_case->node_data[BOUNDARY_DISPLACEMENT][node_id2][dim2];
                            }
                            continue;
                        }
                        indices.emplace_back(dof_id1, dof_id2, mat(idx * n_dof + dim1, idy * n_dof + dim2));
                    }
                }
            }
        }
    }

    Eigen::SparseMatrix<Precision> matrix{max_id,max_id};
    matrix.setFromTriplets(indices.begin(), indices.end());
    return matrix;
}
