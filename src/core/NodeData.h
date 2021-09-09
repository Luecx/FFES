//
// Created by Luecx on 31.08.2021.
//

#ifndef FEM_SRC_CORE_NODEDATA_H_
#define FEM_SRC_CORE_NODEDATA_H_

#include "ComponentContainer.h"
#include "defs.h"

#include <ostream>

enum NodeDataEntries{
    POSITION,                       // coordinates
    VELOCITY,                       // coordinates
    BOUNDARY_IS_CONSTRAINED,        // 1 if boundary displacement is constrained (see below)
    BOUNDARY_DISPLACEMENT,          // boundary displacement, relevant if BOUNDARY_IS_CONSTRAINED = 1
    BOUNDARY_FORCE,                 // boundary force, not relevant if constrained
    DISPLACEMENT,                   // displacement in final solution
    N_MAX_NODE_FLOAT_ENTRIES
};

enum NodeDataIntegerEntries{
    REDUCED_STIFFNESS_INDEX,
    N_MAX_NODE_INT_ENTRIES
};

struct NodeData{
    ComponentContainer<Precision> float_data[N_MAX_NODE_FLOAT_ENTRIES]{};
    ComponentContainer<int>       int_data  [N_MAX_NODE_INT_ENTRIES]{};

    ComponentContainer<Precision>& operator[](NodeDataEntries entry){
        return float_data[entry];
    }

    ComponentContainer<int>&       operator[](NodeDataIntegerEntries entry){
        return int_data[entry];
    }

};

#endif    // FEM_SRC_CORE_NODEDATA_H_
