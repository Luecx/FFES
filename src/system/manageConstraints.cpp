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
#include "../assert/Assert.h"
#include "LoadCase.h"

#include <cmath>
void LoadCase::constraint(const std::string& set,
                Precision x,
                Precision y,
                Precision z,
                bool isTemp){

    ID node_set_id = model->getNodeSetID(set);
    ASSERT(node_set_id >= 0);

    for(ID id:model->node_sets[node_set_id].ids){
        constraint(id, x,y,z, isTemp);
    }
}

void LoadCase::constraint(const std::string& set,
                int dimension,
                Precision value,
                bool isTemp){
    ID node_set_id = model->getNodeSetID(set);
    ASSERT(node_set_id >= 0);

    for(ID id:model->node_sets[node_set_id].ids){
        constraint(id,dimension,value, isTemp);
    }
}
void LoadCase::constraint(int node_id,
                Precision x,
                Precision y,
                Precision z,
                bool isTemp){

    isTemp = previous_load_case != nullptr && isTemp;

    Precision xyz[]{x,y,z};
    for(int i = 0; i < 3; i++){
        constraint(node_id, i, xyz[i], isTemp);
    }
}

void LoadCase::constraint(int node_id,
                int dimension,
                Precision value,
                bool isTemp){

    if(!node_data[BOUNDARY_IS_CONSTRAINED].isInitialised()){
        node_data[BOUNDARY_IS_CONSTRAINED]
            .init(model->max_node_count * 3, model->max_node_count)
            .even(3);
    }

    if(!node_data[BOUNDARY_DISPLACEMENT].isInitialised()){
        node_data[BOUNDARY_DISPLACEMENT]
            .init(model->max_node_count * 3, model->max_node_count)
            .even(3);
    }

    if(!std::isnan(value)){

        node_data[BOUNDARY_IS_CONSTRAINED].set(node_id, dimension, 1    , !isTemp);
        node_data[BOUNDARY_DISPLACEMENT  ].set(node_id, dimension, value, !isTemp);

    }else{

        node_data[BOUNDARY_IS_CONSTRAINED].set(node_id, dimension, 0    , !isTemp);
        node_data[BOUNDARY_DISPLACEMENT  ].set(node_id, dimension, 0    , !isTemp);

    }
}
