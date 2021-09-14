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

void Model::solidSection(const std::string& set, const std::string& material) {
    ID set_id = getElementSetID(set);
    ID mat_id = getMaterialID(material);

    ERROR(set_id >= 0, ELEMENT_SET_NOT_FOUND, set_id);
    ERROR(mat_id >= 0, MATERIAL_NOT_FOUND, mat_id);

    for(auto id:element_sets[set_id].ids){
        elements[id]->material = materials[mat_id];
    }
}

