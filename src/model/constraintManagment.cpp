//
// Created by Luecx on 07.09.2021.
//
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

