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

    WARNING(set_id >= 0, "did not find element set: " << set);
    WARNING(mat_id >= 0, "did not find material: " << material);

    if(set_id < 0 || mat_id < 0) return;

    for(auto id:element_sets[set_id].ids){
        elements[id]->material = materials[mat_id];
    }
}

