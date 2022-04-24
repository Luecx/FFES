
#include "Model.h"


void Model::updateNodeConnectedCount() {
    this->node_data[NODE_CONNECTED_ELEMENTS].init(max_node_count, max_node_count).even(1).fill(0);
    for(auto el:elements){
        if (el == nullptr) continue;
        for(int i = 0; i < el->nodeCount(); i++){
            this->node_data[NODE_CONNECTED_ELEMENTS][el->nodeIDS()[i]][0] += 1;
        }
    }
}