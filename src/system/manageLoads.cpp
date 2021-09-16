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

#ifndef FEM_SRC_MODEL_LOADMANAGMENT_CPP_
#define FEM_SRC_MODEL_LOADMANAGMENT_CPP_

#include "LoadCase.h"

#include <cmath>

void LoadCase::applyLoad(const std::string& set,
                       Precision x,
                       Precision y,
                       Precision z,
                       bool isTemp){

    ID node_set_id = model->getNodeSetID(set);
    ASSERT(node_set_id >= 0);

    for(ID id:model->node_sets[node_set_id].ids){
        applyLoad(id, x,y,z, isTemp);
    }
}
void LoadCase::applyLoad(const std::string& set,
                          int dimension,
                          Precision value,
                          bool isTemp){
    ID node_set_id = model->getNodeSetID(set);
    ASSERT(node_set_id >= 0);

    for(ID id:model->node_sets[node_set_id].ids){
        applyLoad(id,dimension,value, isTemp);
    }
}
void LoadCase::applyLoad(int node_id,
                       Precision x,
                       Precision y,
                       Precision z,
                       bool isTemp){

    isTemp = previous_load_case != nullptr && isTemp;

    Precision xyz[]{x,y,z};
    for(int i = 0; i < 3; i++){
        applyLoad(node_id, i, xyz[i], isTemp);
    }
}

void LoadCase::applyLoad(int node_id,
               int dimension,
               Precision value,
               bool isTemp){
    if(!std::isnan(value)){
        node_data[BOUNDARY_FORCE][node_id][dimension] = value;
        if(!isTemp){
            node_data[BOUNDARY_FORCE_NEXT_CASE][node_id][dimension] = value;
        }
    }
}

#endif    // FEM_SRC_MODEL_LOADMANAGMENT_CPP_
