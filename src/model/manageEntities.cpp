
#include "Model.h"


void Model::setNode(ID id, Precision x, Precision y, Precision z) {

    ERROR(id < max_node_count, PARSING_INVALID_NODE_ID, "id is larger than max_node_count");

    node_data[POSITION][id][0] = x;
    node_data[POSITION][id][1] = y;
    node_data[POSITION][id][2] = z;

    this->node_sets[0              ].ids.push_back(id);
    this->node_sets[active_node_set].ids.push_back(id);
}
