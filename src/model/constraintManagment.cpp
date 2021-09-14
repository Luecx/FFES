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
void Model::constraint(const std::string& set,
                Precision x,
                Precision y,
                Precision z){

    ID node_set_id = getNodeSetID(set);
    ASSERT(node_set_id >= 0);

    for(ID id:node_sets[node_set_id].ids){
        constraint(id, x,y,z);
    }
}
void Model::constraint(int node_id,
                Precision x,
                Precision y,
                Precision z){
    if(!std::isnan(x)){
        node_data[BOUNDARY_IS_CONSTRAINED][node_id][0] = 1;
        node_data[BOUNDARY_DISPLACEMENT  ][node_id][0] = x;
    }

    if(!std::isnan(y)){
        node_data[BOUNDARY_IS_CONSTRAINED][node_id][1] = 1;
        node_data[BOUNDARY_DISPLACEMENT  ][node_id][1] = y;
    }

    if(!std::isnan(z)){
        node_data[BOUNDARY_IS_CONSTRAINED][node_id][2] = 1;
        node_data[BOUNDARY_DISPLACEMENT  ][node_id][2] = z;
    }
}
