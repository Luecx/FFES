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

void Model::postProcessDisplacements(const Eigen::VectorXd& displacement) {
    for(int i = 0; i < node_count; i++){
        for(int d = 0; d < nodal_dimension; d++){

            auto dof_id = node_data[REDUCED_STIFFNESS_INDEX][i][d];

            if(dof_id < 0){
                node_data[DISPLACEMENT][i][d] = node_data[BOUNDARY_DISPLACEMENT][i][d];
            }else{
                node_data[DISPLACEMENT][i][d] = displacement(dof_id);
            }

        }
    }
}