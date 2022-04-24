
//
// Created by Luecx on 03.10.2021.
//
#include "../system/LoadCase.h"
#include "Model.h"

void Model::computeStressAtNodes(LoadCase* load_case) {
    int stress_dimension = nodal_dimension == 2 ? 3 : 6;
    load_case->node_data[NODAL_STRESS]
        .init(stress_dimension * max_node_count, max_node_count)
        .even(stress_dimension)
        .fill(0);

    for (const ElementPtr& element : elements) {
        if (element == nullptr)
            continue;
        auto mat = element->computeStressAtNodes(load_case);

        for (int i = 0; i < element->nodeCount(); i++) {
            ID id = element->nodeIDS()[i];
            for (int j = 0; j < stress_dimension; j++) {
                load_case->node_data[NODAL_STRESS][id][j] +=
                    mat(i, j) / this->node_data[NODE_CONNECTED_ELEMENTS][id][0];
            }
        }
    }
}