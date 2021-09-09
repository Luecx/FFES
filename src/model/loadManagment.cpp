//
// Created by Luecx on 08.09.2021.
//

#ifndef FEM_SRC_MODEL_LOADMANAGMENT_CPP_
#define FEM_SRC_MODEL_LOADMANAGMENT_CPP_
#include "Model.h"

void Model::applyLoad(const std::string& set,
                       Precision x,
                       Precision y,
                       Precision z){

    ID node_set_id = getNodeSetID(set);
    ASSERT(node_set_id >= 0);

    for(ID id:node_sets[node_set_id].ids){
        applyLoad(id, x,y,z);
    }
}
void Model::applyLoad(int node_id,
                       Precision x,
                       Precision y,
                       Precision z){


    if(!std::isnan(x)){
        node_data[BOUNDARY_FORCE][node_id][0] = x;
    }

    if(!std::isnan(y)){
        node_data[BOUNDARY_FORCE][node_id][1] = y;
    }

    if(!std::isnan(z)){
        node_data[BOUNDARY_FORCE][node_id][2] = z;
    }
}


#endif    // FEM_SRC_MODEL_LOADMANAGMENT_CPP_
