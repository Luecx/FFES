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

ID Model::activateNodeSet(const std::string& name) {
    ID id = getNodeSetID(name);
    if (id == -1) {
        this->node_sets.push_back(Set {name});
        this->active_node_set = this->node_sets.size() - 1;
        return this->active_node_set;
    } else {
        this->active_node_set = id;
        return id;
    }
}
ID Model::activateElementSet(const std::string& name) {
    ID id = getElementSetID(name);
    if (id == -1) {
        this->element_sets.push_back(Set {name});
        this->active_element_set = this->element_sets.size() - 1;
        return this->active_element_set;
    } else {
        this->active_element_set = id;
        return id;
    }
}
ID Model::getNodeSetID(const std::string& name) {
    for (auto i = 0; i < (int) node_sets.size(); i++) {
        if (node_sets[i].name == name) {
            return i;
        }
    }
    return -1;
}
ID Model::getElementSetID(const std::string& name) {
    for (auto i = 0; i < (int) element_sets.size(); i++) {
        if (element_sets[i].name == name) {
            return i;
        }
    }
    return -1;
}

void Model::addNodeToNodeSet(const std::string& name, ID node_id) {
    auto set_id = getNodeSetID(name);
    if (set_id < 0)
        return;
    node_sets[set_id].ids.push_back(node_id);
}
void Model::addElementToElementSet(const std::string& name, ID element_id) {
    auto set_id = getElementSetID(name);
    if (set_id < 0)
        return;
    element_sets[set_id].ids.push_back(element_id);
}
