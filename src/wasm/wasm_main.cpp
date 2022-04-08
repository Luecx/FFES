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
// Created by Luecx on 07.04.2022.
//
//
#define __EMSCRIPTEN__
#ifdef __EMSCRIPTEN__


#include "../core/NodeData.h"
#include "../entity/Iso2DQuad4.h"
#include "../entity/Iso2DQuad8.h"
#include "../entity/Iso2DTri3.h"
#include "../entity/Iso2DTri6.h"
#include "../entity/Iso3DHex20.h"
#include "../entity/Iso3DHex8.h"
#include "../entity/IsoElement.h"
#include "../material/IsotropicElasticity.h"
#include "../model/Model.h"
#include "../reader/Reader.h"
#include <thread>

#include <iostream>

#include <emscripten/emscripten.h>
#ifdef __cplusplus
extern "C" {
#endif

struct ResultEntry {
    std::vector<float> values {};

    ResultEntry(int entries) { values.resize(entries); }

    float& operator[](int idx) { return values[idx]; }
    float  operator[](int idx) const { return values[idx]; }

    void   storeInArray(float* target) {
        std::memcpy(target, values.begin().base(), sizeof(float) * values.size());
    }
};

struct Result {
    std::vector<ResultEntry> entries {};

    ResultEntry& operator[](int idx) {
        if (idx >= entries.size()) {
            entries.reserve(idx + 1);
        }
        return entries[idx];
    }
};

Result wasm_result_stress_x;
Result wasm_result_stress_y;
Result wasm_result_stress_xy;
Result wasm_result_stress_mises;
Result wasm_result_displacement_x;
Result wasm_result_displacement_y;
Result wasm_result_displacement_xy;

System* wasm_build_system(const int32_t n_nodes,
                          const int32_t n_elems,
                          const float*  node_coords,
                          const float*  elem_node_ids,
                          const int32_t nodes_per_element,
                          const float*  material,
                          const float*  restricted,
                          const float*  displacement,
                          const float*  loads){
    System* system = new System((int)n_nodes, (int)n_elems);

    // generate nodes first
    for(int i = 0; i < n_nodes; i++){
        system->model.setNode(i, node_coords[i * 2 + 0],
                              node_coords[i * 2 + 1]);
    }

    // generate elements
    for (int i = 0; i < n_elems; i++) {
        if(nodes_per_element == 3){
            system->model.setElement<Iso2DTri3 >(i,(int)elem_node_ids[i * nodes_per_element + 0],
                                                (int)elem_node_ids[i * nodes_per_element + 1],
                                                (int)elem_node_ids[i * nodes_per_element + 2]);
        }else if(nodes_per_element == 4){
            system->model.setElement<Iso2DQuad4>(i,(int)elem_node_ids[i * nodes_per_element + 0],
                                                 (int)elem_node_ids[i * nodes_per_element + 1],
                                                 (int)elem_node_ids[i * nodes_per_element + 2],
                                                 (int)elem_node_ids[i * nodes_per_element + 3]);
        }else if(nodes_per_element == 8){
            system->model.setElement<Iso2DQuad8>(i,(int)elem_node_ids[i * nodes_per_element + 0],
                                                 (int)elem_node_ids[i * nodes_per_element + 1],
                                                 (int)elem_node_ids[i * nodes_per_element + 2],
                                                 (int)elem_node_ids[i * nodes_per_element + 3],
                                                 (int)elem_node_ids[i * nodes_per_element + 4],
                                                 (int)elem_node_ids[i * nodes_per_element + 5],
                                                 (int)elem_node_ids[i * nodes_per_element + 6],
                                                 (int)elem_node_ids[i * nodes_per_element + 7]);
        }
    }

    // material
    system->model.addMaterial("mat1");
    system->model.materials[0].setElasticity<IsotropicElasticity>(material[0],material[1]);
    system->model.solidSection("EALL", "mat1", 1);

    // constraints and loads
    for(int i = 0; i < n_nodes * 2; i++){
        int node = i / 2;
        int dim  = i % 2;
        if(restricted[i] > 0){
            system->getLoadCase()->constraint(node, dim, displacement[i]);
        }
        if(loads[i] != 0){
            system->getLoadCase()->applyLoad(node, dim, loads[i]);
        }
    }
}

void wasm_save(System* system, int id){
    // put results into arrays above
    wasm_result_stress_x       [id] = ResultEntry(system->model.max_node_count);
    wasm_result_stress_y       [id] = ResultEntry(system->model.max_node_count);
    wasm_result_stress_xy      [id] = ResultEntry(system->model.max_node_count);
    wasm_result_stress_mises   [id] = ResultEntry(system->model.max_node_count);
    wasm_result_displacement_x [id] = ResultEntry(system->model.max_node_count);
    wasm_result_displacement_y [id] = ResultEntry(system->model.max_node_count);
    wasm_result_displacement_xy[id] = ResultEntry(system->model.max_node_count);
    
    for (int i = 0; i < system->model.max_node_count; i++) {
        if (system->model.node_data[NODE_CONNECTED_ELEMENTS][i][0] == 0)
            continue;

        Precision dx  = system->getLoadCase()->node_data[DISPLACEMENT][i][0];
        Precision dy  = system->getLoadCase()->node_data[DISPLACEMENT][i][1];
        Precision dxy = std::sqrt(dx * dx + dy * dy);
        Precision sx  = system->getLoadCase()->node_data[NODAL_STRESS][i][0];
        Precision sy  = system->getLoadCase()->node_data[NODAL_STRESS][i][1];
        Precision sxy = system->getLoadCase()->node_data[NODAL_STRESS][i][2];
        Precision mis = std::sqrt(sx * sx + sy * sy - sx * sy + 3 * sxy * sxy);

        wasm_result_displacement_x[id][i]  = dx;
        wasm_result_displacement_y[id][i]  = dy;
        wasm_result_displacement_xy[id][i] = dxy;
        wasm_result_stress_x[id][i]        = sx;
        wasm_result_stress_y[id][i]        = sy;
        wasm_result_stress_xy[id][i]       = sxy;
        wasm_result_stress_mises[id][i]    = mis;
    }
}


EMSCRIPTEN_KEEPALIVE void wasm_analysis(const int32_t n_nodes,
                                        const int32_t n_elems,
                                        const float*  node_coords,
                                        const float*  elem_node_ids,
                                        const int32_t nodes_per_element,
                                        const float*  material,
                                        const float*  restricted,
                                        const float*  displacement,
                                        const float*  loads) {

    System* system = wasm_build_system(n_nodes,
                                       n_elems,
                                       node_coords,
                                       elem_node_ids,
                                       nodes_per_element,
                                       material,
                                       restricted,
                                       displacement,
                                       loads);

    std::thread th {[=]() {
        // compute
        system->getLoadCase()->compute();

        // put results into arrays above
        wasm_save(system, 0);
        delete system;
        std::cout << "Finished computation" << std::endl;
    }};
    th.detach();
}
void copy_to_array(std::vector<float> &vec, float* res){
    std::memcpy(res, vec.begin().base(), sizeof(float) * vec.size());
}
EMSCRIPTEN_KEEPALIVE void wasm_get_displacement_x(float* res){
    copy_to_array(wasm_result_displacement_x, res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_displacement_y(float* res){
    copy_to_array(wasm_result_displacement_y, res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_displacement_xy(float* res){
    copy_to_array(wasm_result_displacement_xy, res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_stress_x(float* res){
    copy_to_array(wasm_result_stress_x, res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_stress_y(float* res){
    copy_to_array(wasm_result_stress_y, res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_stress_xy(float* res){
    copy_to_array(wasm_result_stress_xy, res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_stress_mises(float* res){
    copy_to_array(wasm_result_stress_mises, res);
}
#ifdef __cplusplus
}
#endif
#endif