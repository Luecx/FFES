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

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#ifndef __EMSCRIPTEN__
#define EMSCRIPTEN_KEEPALIVE
#define __EMSCRIPTEN__
#endif

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

#ifdef __EMSCRIPTEN__
#ifdef __cplusplus
extern "C" {
#endif
#endif

struct ResultEntry {
    std::vector<float> values {};

    ResultEntry() { }
    ResultEntry(int entries) { values.resize(entries); }
    ResultEntry(ResultEntry&& other)  noexcept : values{std::move(other.values)}{ }
    ResultEntry(const ResultEntry& other) : values{other.values}{ }
    ResultEntry& operator=(ResultEntry&& other){
        this->values = std::move(other.values);
        return *this;
    }
    ResultEntry& operator=(const ResultEntry& other){
        this->values = other.values;
        return *this;
    }

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
            entries.resize(idx + 1);
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
Result wasm_result_density;


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
    return system;
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
    wasm_result_density        [id] = ResultEntry(system->model.max_node_count);

    
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

    // densities are averaged from the elements
    if(system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR].isInitialised()){
        for(const auto& el:system->model.elements){
            if(el == nullptr) continue;

            for(int i = 0; i < el->nodeCount(); i++){
                int node_id = el->nodeIDS()[i];
                wasm_result_density[id][node_id] +=
                    system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][el->element_id][0] /
                    system->model.node_data[NODE_CONNECTED_ELEMENTS][node_id][0];
            }
        }
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

    // if simp is still enabled from previous runs, disable it


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

EMSCRIPTEN_KEEPALIVE void wasm_topo    (
                                        const int32_t n_nodes,
                                        const int32_t n_elems,
                                        const float*  node_coords,
                                        const float*  elem_node_ids,
                                        const int32_t nodes_per_element,
                                        const float*  material,
                                        const float*  restricted,
                                        const float*  displacement,
                                        const float*  loads,

                                        const float target_density,
                                        const float simp_exponent,
                                        const float r_min,
                                        const float min_density,
                                        const float move_limit,
                                        const int32_t max_iterations){

    // build the system
    System* system = wasm_build_system(n_nodes,
                                       n_elems,
                                       node_coords,
                                       elem_node_ids,
                                       nodes_per_element,
                                       material,
                                       restricted,
                                       displacement,
                                       loads);

    std::cout << "target_density: " << target_density << std::endl;
    std::cout << "simp_exponent:  " << simp_exponent << std::endl;
    std::cout << "r_min:          " << r_min << std::endl;
    std::cout << "min_density:    " << min_density << std::endl;
    std::cout << "move_limit:     " << move_limit << std::endl;
    std::cout << "max_iterations: " << max_iterations << std::endl;

    std::thread th {[=]() {
        // enable simp
        system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR  ].init(n_elems, n_elems).even(1).fill(target_density);
        system->getLoadCase()->element_data[SIMP_DENSITY_EXPONENT].init(n_elems, n_elems).even(1).fill(simp_exponent);

        ComponentContainer<Precision> sensitivities         {};
        ComponentContainer<Precision> filtered_sensitivities{};
        ComponentContainer<Precision> element_center        {};
        ComponentContainer<Precision> element_volume        {};
        ComponentContainer<Precision> prev_density          {};
        sensitivities         .init(n_elems    , n_elems).even(1).fill(0);
        filtered_sensitivities.init(n_elems    , n_elems).even(1).fill(0);
        element_center        .init(n_elems * 2, n_elems).even(2).fill(0);
        element_volume        .init(n_elems    , n_elems).even(1).fill(0);
        prev_density          .init(n_elems    , n_elems).even(1).fill(0);


        // initialise center of mass
        for(auto el:system->model.elements){
            auto node_coords = el->getNodalData(POSITION, 2);
            auto com         = QuickMatrix<2,1>();

            for(int i = 0; i < nodes_per_element; i++){
                com(0,0) += node_coords(i * 2    , 0);
                com(1,0) += node_coords(i * 2 + 1, 0);
            }
            element_center[el->element_id][0] = com(0,0) / nodes_per_element;
            element_center[el->element_id][1] = com(1,0) / nodes_per_element;

            element_volume[el->element_id][0] = el->volume();
        }

        for(int it = 0; it < max_iterations; it++){
            // track change of densities --> save previous densities
            for(const auto& el:system->model.elements){
                // skip non existing elements
                if (el == nullptr)
                    continue;
                prev_density[el->element_id][0] =
                    system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][el->element_id][0];
            }

            // compute
            system->getLoadCase()->compute();

            // get compliance
            Precision compliance = 0;
            for(const auto& el:system->model.elements){
                // skip non existing elements
                if (el == nullptr)
                    continue;
                // get the compliance of the element
                auto element_compliance = el->compliance(system->getLoadCase());
                // get the current density
                Precision current_density =
                    system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][el->element_id][0];
                // set sensitivity for the element
                sensitivities[el->element_id] =
                    -simp_exponent * std::pow(current_density, simp_exponent - 1) * element_compliance;
                compliance += element_compliance;
            }

            // mesh independency filter
            filtered_sensitivities.fill(0);
            for(const auto& elem:system->model.elements){
                auto x1 = element_center[elem->element_id][0];
                auto y1 = element_center[elem->element_id][1];

                auto elem_id = elem->element_id;
                auto density = system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][elem_id][0];

                // sum in the denominator
                Precision sum = 0;
                for(const auto& other:system->model.elements){

                    auto x2 = element_center[other->element_id][0];
                    auto y2 = element_center[other->element_id][1];

                    auto dist = std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
                    auto wgt  = std::max(r_min - dist, 0.0);

                    sum += wgt;
                    filtered_sensitivities[elem_id][0] += wgt * density * sensitivities[other->element_id][0];
                }
                filtered_sensitivities[elem_id][0] /= density * sum;
            }

            // apply optimality criterion
            Precision lag_1 = 0;
            Precision lag_2 = 1e9;
            while((lag_2-lag_1) > 1e-12){
                Precision lag_mid = (lag_2 + lag_1) / 2;

                Precision mass = 0;
                Precision volume = 0;

                // adjust values in x
                for(const auto& elem:system->model.elements) {
                    auto elem_id = elem->element_id;
                    auto density = prev_density[elem->element_id][0];
                    auto sens    = filtered_sensitivities[elem_id][0];

                    auto change  = density * sqrt(-sens / lag_mid);
                    auto move_clamp = std::clamp(change, density - move_limit, density + move_limit);
                    auto boun_clamp = std::clamp(move_clamp, (Precision) min_density, 1.0);

                    auto new_density = boun_clamp;
                    system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][elem_id][0] = new_density;

                    // compute total volume and total mass
                    auto elem_vol = element_volume[elem_id][0];
                    mass += new_density * elem_vol;
                    volume += elem_vol;
                }

                // adjust lagrange multiplier
                if((mass - target_density * volume) > 0){
                    lag_1 = lag_mid;
                }else{
                    lag_2 = lag_mid;
                }
            }

            // compute difference
            Precision change = 0;
            for(const auto& el:system->model.elements){
                // skip non existing elements
                if (el == nullptr)
                    continue;
                change += std::abs(prev_density[el->element_id][0] -
                    system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][el->element_id][0]);
            }

            // end if the change is small enough
            if(change < 0.0001){
                break;
            }


            wasm_save(system, it+1);
            std::cout << "Iteration " << (it+1) << " finished. Compliance: " << compliance << std::endl;
        }

        std::cout << "Finished computation" << std::endl;

        // free the system
        delete system;
    }};
    th.detach();
}

EMSCRIPTEN_KEEPALIVE void wasm_get_displacement_x(float* res, int it){
    wasm_result_displacement_x[it].storeInArray(res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_displacement_y(float* res, int it){
    wasm_result_displacement_y[it].storeInArray(res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_displacement_xy(float* res, int it){
    wasm_result_displacement_xy[it].storeInArray(res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_stress_x(float* res, int it){
    wasm_result_stress_x[it].storeInArray(res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_stress_y(float* res, int it){
    wasm_result_stress_y[it].storeInArray(res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_stress_xy(float* res, int it){
    wasm_result_stress_xy[it].storeInArray(res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_stress_mises(float* res, int it){
    wasm_result_stress_mises[it].storeInArray(res);
}
EMSCRIPTEN_KEEPALIVE void wasm_get_density(float* res, int it){
    wasm_result_density[it].storeInArray(res);
}
#ifdef __EMSCRIPTEN__
#ifdef __cplusplus
}
#endif
#endif
