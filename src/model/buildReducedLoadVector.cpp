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
Eigen::VectorXd Model::buildReducedLoadVector() {
    ID           max_id = this->numerateUnconstrainedNodes();

    Eigen::VectorXd  nodal_forces {max_id, 1};

    ASSERT(node_data[BOUNDARY_FORCE         ].isInitialised());
    ASSERT(node_data[REDUCED_STIFFNESS_INDEX].isInitialised());

    // first apply nodal forces
    for(int i = 0; i < node_count; i++){
        for(int j = 0; j < nodal_dimension; j++){
            int dof_id1 = node_data[REDUCED_STIFFNESS_INDEX][i][j];

            if (dof_id1 < 0) continue;

            nodal_forces(dof_id1) = node_data[BOUNDARY_FORCE][i][j]
                                       + node_data[BOUNDARY_IMPLIED_DISPLACEMENT_FORCE][i][j];
        }
    }

    return nodal_forces;
}