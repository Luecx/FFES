/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFES is free software: you can redistribute it and/or modify                  *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFES is distributed in the hope that it will be useful,                    *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/
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

    for (Element* element : elements) {
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