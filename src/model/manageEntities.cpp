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
#include "Model.h"


void Model::setNode(ID id, Precision x, Precision y, Precision z) {

    ERROR(id < max_node_count, PARSING_INVALID_NODE_ID, "id is larger than max_node_count");

    node_data[POSITION][id][0] = x;
    node_data[POSITION][id][1] = y;
    node_data[POSITION][id][2] = z;

    this->node_sets[0              ].ids.push_back(id);
    this->node_sets[active_node_set].ids.push_back(id);
}
