
#include "LoadCase.h"

void LoadCase::reconstructBoundary(){
    if(previous_load_case == nullptr) return;

    for(int i = 0; i < N_MAX_NODE_FLOAT_ENTRIES; i++){
        auto idx = (NodeDataEntries) i;
        node_data[idx] = previous_load_case->node_data[idx];
    }
    for(int i = 0; i < N_MAX_NODE_INT_ENTRIES; i++){
        auto idx = (NodeDataIntegerEntries) i;
        node_data[idx] = previous_load_case->node_data[idx];
    }
    for(int i = 0; i < N_MAX_ELEMENT_FLOAT_ENTRIES; i++){
        auto idx = (ElementDataEntries) i;
        element_data[idx] = previous_load_case->element_data[idx];
    }
    for(int i = 0; i < N_MAX_ELEMENT_INT_ENTRIES; i++){
        auto idx = (ElementDataIntegerEntries) i;
        element_data[idx] = previous_load_case->element_data[idx];
    }
}
