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