//#include "core/NodeData.h"
//#include "entity/Iso2DQuad4.h"
//#include "entity/Iso2DQuad8.h"
//#include "entity/Iso2DTri3.h"
//#include "entity/Iso2DTri6.h"
//#include "entity/Iso3DHex20.h"
//#include "entity/Iso3DHex8.h"
//#include "entity/IsoElement.h"
//#include "material/IsotropicElasticity.h"
//#include "model/Model.h"
//#include "reader/Reader.h"
//#include "delaunay/Node.h"
//#include "delaunay/Edge.h"
//#include "delaunay/Triangle.h"
//#include "delaunay/Mesher2D.h"
//#include <random>
//#include <thread>
//
//#include <iostream>
//
//
//
////void testC2D8(){
////        // folding for C2D8
////        System system{50,5};
////        int element_count = 3;
////        Precision element_size  = 10;
////        int node_id = 0;
////        for(int i = 0; i < element_count+1; i++){
////            system.model.setNode(node_id ++, i * element_size                 , -element_size/2);
////            system.model.setNode(node_id ++, i * element_size + element_size/2, -element_size/2);
////            system.model.setNode(node_id ++, i * element_size + element_size/2, +element_size/2);
////            system.model.setNode(node_id ++, i * element_size                 , +element_size/2);
////            system.model.setNode(node_id ++, i * element_size                 , 0);
////        }
////        std::cout << system.model.node_data[POSITION] << std::endl;
////        for(int i = 0; i < element_count; i++){
////            int index_1 = i * 5;
////            int index_2 = (i+1) * 5;
////            system.model.setElement<Iso2DQuad8>(i,
////                                                index_1,
////                                                index_2,
////                                                index_2+3,
////                                                index_1+3,
////                                                index_1+1,
////                                                index_2+4,
////                                                index_1+2,
////                                                index_1+4);
////        }
////        // constraint left nodes
////        system.getLoadCase()->constraint(0,0,0,0,false);
////        system.getLoadCase()->constraint(3,0,0,0,false);
////        system.getLoadCase()->constraint(4,0,0,0,false);
////        // load right top? node
////        system.getLoadCase()->applyLoad(element_count*5+3,1,10);
////        // create section
////        system.model.addMaterial("mat1");
////        system.model.materials[0].setElasticity<IsotropicElasticity>(70000,0);
////        system.model.solidSection("EALL", "mat1", 1);
////
////        system.getLoadCase()->compute();
////
////        for(int i = 0; i < system.model.max_node_count;i++){
////            if(system.model.node_data[NODE_CONNECTED_ELEMENTS][i][0] == 0) continue;
////            std::cout << system.model.node_data[POSITION][i][0] << ","
////                      << system.model.node_data[POSITION][i][1] << ","
////                      << system.model.node_data[POSITION][i][2] << ",";
////            std::cout << system.getLoadCase()->node_data[DISPLACEMENT][i][0] << ","
////                      << system.getLoadCase()->node_data[DISPLACEMENT][i][1] << ","
////                      << system.getLoadCase()->node_data[DISPLACEMENT][i][2] << ",";
////            std::cout << system.getLoadCase()->node_data[NODAL_STRESS][i][0] << ","
////                      << system.getLoadCase()->node_data[NODAL_STRESS][i][1] << ","
////                      << system.getLoadCase()->node_data[NODAL_STRESS][i][2] << "\n";
////        }
////}
////
////void testC3D20(){
////    System system{2000,100};
////
////        // folding for C3D20
////
////        int element_count = 6;
////        int element_size  = 2;
////
////        auto half = element_size / 2;
////
////        int node_id = 0;
////        for(int i = 0; i < 100; i++){
////            // create nodes
////            system.model.setNode(node_id++,-(-half) , -half,(i * element_size       ) );
////            system.model.setNode(node_id++,-(-half) ,-half ,(i * element_size + half) );
////            system.model.setNode(node_id++,-(-half) , half ,(i * element_size + half) );
////            system.model.setNode(node_id++,-(-half) ,  half,(i * element_size       ) );
////            system.model.setNode(node_id++,-(-half) ,   0.0,(i * element_size       ) );
////            system.model.setNode(node_id++,-( half) , -half,(i * element_size       ) );
////            system.model.setNode(node_id++,-( half) ,-half ,(i * element_size + half) );
////            system.model.setNode(node_id++,-( half) , half ,(i * element_size + half) );
////            system.model.setNode(node_id++,-( half) ,  half,(i * element_size       ) );
////            system.model.setNode(node_id++,-( half) ,   0.0,(i * element_size       ) );
////            system.model.setNode(node_id++,-(  0.0) , -half,(i * element_size       ) );
////            system.model.setNode(node_id++,-(  0.0) ,  half,(i * element_size       ) );
////        }
////
////        for(int j = 0; j < element_count; j++) {
////            int first_index  =  j      * 12;
////            int second_index = (j + 1) * 12;
////            system.model.setElement<Iso3DHex20>(
////                j,
////                first_index,
////                second_index,
////                second_index + 3,
////                first_index + 3,
////                first_index + 5,
////                second_index + 5,
////                second_index + 8,
////                first_index + 8,
////
////                first_index + 1,
////                second_index + 4,
////                first_index + 2,
////                first_index + 4,
////
////                first_index + 6,
////                second_index + 9,
////                first_index + 7,
////                first_index + 9,
////
////                first_index + 10,
////                second_index + 10,
////                second_index + 11,
////                first_index + 11);
////        }
////
////        system.getLoadCase()->constraint(0,0,0,0,true);
////        system.getLoadCase()->constraint(3,0,0,0,true);
////        system.getLoadCase()->constraint(4,0,0,0,true);
////        system.getLoadCase()->constraint(5,0,0,0,true);
////        system.getLoadCase()->constraint(8,0,0,0,true);
////        system.getLoadCase()->constraint(9,0,0,0,true);
////        system.getLoadCase()->constraint(10,0,0,0,true);
////        system.getLoadCase()->constraint(11,0,0,0, true);
////
////        // twisting
////    //    system.getLoadCase()->applyLoad(element_count * 12 + 0 ,0,1,false);
////    //    system.getLoadCase()->applyLoad(element_count * 12 + 3 ,1,1,false);
////    //    system.getLoadCase()->applyLoad(element_count * 12 + 5 ,1,-1,false);
////    //    system.getLoadCase()->applyLoad(element_count * 12 + 8 ,0,-1,false);
////
////        // bending
////        system.getLoadCase()->applyLoad(element_count * 12 + 0 ,0,1,false);
////        system.getLoadCase()->applyLoad(element_count * 12 + 3 ,0,1,false);
////        system.getLoadCase()->applyLoad(element_count * 12 + 5 ,0,1,false);
////        system.getLoadCase()->applyLoad(element_count * 12 + 8 ,0,1,false);
////
////    //    system.getLoadCase()->applyLoad(element_count * 12 + 4 ,0,10* 1.0/4 ,false);
////    //    system.getLoadCase()->applyLoad(element_count * 12 + 11,0,10* 1.0/4 ,false);
////    //    system.getLoadCase()->applyLoad(element_count * 12 + 9 ,0,10* 1.0/4 ,false);
////    //    system.getLoadCase()->applyLoad(element_count * 12 + 10,0,10* 1.0/4 ,false);
////
////}
////
////void testShapeFunction() {
////    Iso3DHex8   quad {1, 2, 3, 4, 5, 6, 7, 8};
////
////    // check shape function
////    auto        local_coords = quad.getNodeLocalCoordinates();
////    DenseMatrix node_shape_values {quad.nodeCount(), quad.nodeCount()};
////    for (int i = 0; i < quad.nodeCount(); i++) {
////        auto r     = local_coords(i, 0);
////        auto s     = local_coords(i, 1);
////        auto t     = local_coords(i, 2);
////        auto shape = quad.getShapeFunction(r, s, t);
////        for (int j = 0; j < quad.nodeCount(); j++) {
////            node_shape_values(i, j) = shape(j, 0);
////        }
////    }
////    std::cout << "------------------------------------------------------------------" << std::endl;
////    std::cout << "has to be identity matrix:" << std::endl;
////    std::cout << node_shape_values << std::endl;
////
////    // verify derivative
////    Precision delta = 0.001;
////    for (Precision r = -1; r < 1; r += 0.4) {
////        for (Precision s = -1; s < 1; s += 0.4) {
////            for (Precision t = -1; t < 1; t += 0.4) {
////                auto derivative    = quad.getLocalShapeDerivative(r, s, t);
////
////                auto shape_func_rp = quad.getShapeFunction(r + delta, s, t);
////                auto shape_func_rm = quad.getShapeFunction(r - delta, s, t);
////                auto shape_func_sp = quad.getShapeFunction(r, s + delta, t);
////                auto shape_func_sm = quad.getShapeFunction(r, s - delta, t);
////                auto shape_func_tp = quad.getShapeFunction(r, s, t + delta);
////                auto shape_func_tm = quad.getShapeFunction(r, s, t - delta);
////                for (int node = 0; node < quad.nodeCount(); node++) {
////                    Precision grad_r =
////                        (shape_func_rp(node, 0) - shape_func_rm(node, 0)) / (2 * delta);
////                    Precision grad_s =
////                        (shape_func_sp(node, 0) - shape_func_sm(node, 0)) / (2 * delta);
////                    Precision grad_t =
////                        (shape_func_tp(node, 0) - shape_func_tm(node, 0)) / (2 * delta);
////                    if (std::abs(grad_r - derivative(0, node)) > 0.0001) {
////                        std::cout << grad_r << "  " << derivative(0, node) << std::endl;
////                    }
////                    if (std::abs(grad_s - derivative(1, node)) > 0.0001) {
////                        std::cout << grad_s << "  " << derivative(1, node) << std::endl;
////                    }
////                    if (std::abs(grad_t - derivative(2, node)) > 0.0001) {
////                        std::cout << grad_t << "  " << derivative(2, node) << std::endl;
////                    }
////                }
////            }
////    }
////}
//
////template <
////    class result_t   = std::chrono::microseconds,
////    class clock_t    = std::chrono::steady_clock,
////    class duration_t = std::chrono::microseconds
////    >
////auto since(std::chrono::time_point<clock_t, duration_t> const& start)
////{
////    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
////}
////
//
//
//void wasm_topo_opt(System* system,
//                   const float target_density,
//                   const float simp_exponent,
//                   const float r_min,
//                   const float min_density,
//                   const float move_limit,
//                   const int32_t max_iterations){
//
//    std::cout << "target_density: " << target_density << std::endl;
//    std::cout << "simp_exponent:  " << simp_exponent << std::endl;
//    std::cout << "r_min:          " << r_min << std::endl;
//    std::cout << "min_density:    " << min_density << std::endl;
//    std::cout << "move_limit:     " << move_limit << std::endl;
//    std::cout << "max_iterations: " << max_iterations << std::endl;
//
//    int n_elems = system->model.max_element_count;
//    int n_nodes = system->model.max_node_count;
//    int nodes_per_element = system->model.elements[0]->nodeCount();
//
//    std::thread th {[=]() {
//        // enable simp
//        system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR  ].init(n_elems, n_elems).even(1).fill(target_density);
//        system->getLoadCase()->element_data[SIMP_DENSITY_EXPONENT].init(n_elems, n_elems).even(1).fill(simp_exponent);
//
//        ComponentContainer<Precision> sensitivities         {};
//        ComponentContainer<Precision> filtered_sensitivities{};
//        ComponentContainer<Precision> element_center        {};
//        ComponentContainer<Precision> element_volume        {};
//        ComponentContainer<Precision> prev_density          {};
//        sensitivities         .init(n_elems    , n_elems).even(1).fill(0);
//        filtered_sensitivities.init(n_elems    , n_elems).even(1).fill(0);
//        element_center        .init(n_elems * 2, n_elems).even(2).fill(0);
//        element_volume        .init(n_elems    , n_elems).even(1).fill(0);
//        prev_density          .init(n_elems    , n_elems).even(1).fill(0);
//
//        // initialise center of mass
//        for(auto el:system->model.elements){
//            auto node_coords = el->getNodalData(POSITION, 2);
//            auto com         = QuickMatrix<2,1>();
//
//            for(int i = 0; i < nodes_per_element; i++){
//                com(0,0) += node_coords(i * 2    , 0);
//                com(1,0) += node_coords(i * 2 + 1, 0);
//            }
//            element_center[el->element_id][0] = com(0,0) / nodes_per_element;
//            element_center[el->element_id][1] = com(1,0) / nodes_per_element;
//
//            element_volume[el->element_id][0] = el->volume();
//        }
//
//        for(int it = 0; it < max_iterations; it++){
//            // track change of densities --> save previous densities
//            for(const auto& el:system->model.elements){
//                // skip non existing elements
//                if (el == nullptr)
//                    continue;
//                prev_density[el->element_id][0] =
//                    system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][el->element_id][0];
//            }
//
//            // compute
//            system->getLoadCase()->compute();
//
//            // get compliance
//            Precision compliance = 0;
//            for(const auto& el:system->model.elements){
//                // skip non existing elements
//                if (el == nullptr)
//                    continue;
//                // get the compliance of the element
//                auto element_compliance = el->compliance(system->getLoadCase());
//                // get the current density
//                Precision current_density =
//                    system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][el->element_id][0];
//                // set sensitivity for the element
//                sensitivities[el->element_id] =
//                    -simp_exponent * std::pow(current_density, simp_exponent - 1) * element_compliance;
//                compliance += element_compliance;
//            }
//            // mesh independency filter
//            filtered_sensitivities.fill(0);
//            for(const auto& elem:system->model.elements){
//                auto x1 = element_center[elem->element_id][0];
//                auto y1 = element_center[elem->element_id][1];
//
//                auto elem_id = elem->element_id;
//                auto density = system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][elem_id][0];
//
//                // sum in the denominator
//                Precision sum = 0;
//                for(const auto& other:system->model.elements){
//
//                    auto x2 = element_center[other->element_id][0];
//                    auto y2 = element_center[other->element_id][1];
//
//                    auto dist = std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
//                    auto wgt  = std::max(r_min - dist, 0.0);
//
//                    sum += wgt;
//                    filtered_sensitivities[elem_id][0] += wgt * density * sensitivities[other->element_id][0];
//                }
//                filtered_sensitivities[elem_id][0] /= density * sum;
//            }
//            // apply optimality criterion
//            Precision lag_1 = 0;
//            Precision lag_2 = 1e18;
//            while((lag_2-lag_1) > 1e-10){
//                Precision lag_mid = (lag_2 + lag_1) / 2;
//
////                std::cout << lag_mid << std::endl;
////                std::cout << lag_2 << std::endl;
////                std::cout << lag_1 << std::endl;
//
//                Precision mass = 0;
//                Precision volume = 0;
//
//                // adjust values in x
//                for(const auto& elem:system->model.elements) {
//                    auto elem_id = elem->element_id;
//                    auto density = prev_density[elem->element_id][0];
//                    auto sens    = filtered_sensitivities[elem_id][0];
//
//                    auto change  = density * sqrt(-sens / lag_mid);
//                    auto move_clamp = std::clamp(change, density - move_limit, density + move_limit);
//                    auto boun_clamp = std::clamp(move_clamp, (Precision) min_density, 1.0);
//
//                    auto new_density = boun_clamp;
//                    system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][elem_id][0] = new_density;
//
//                    // compute total volume and total mass
//                    auto elem_vol = element_volume[elem_id][0];
//                    mass += new_density * elem_vol;
//                    volume += elem_vol;
//                }
//
//                // adjust lagrange multiplier
//                if((mass - target_density * volume) > 0){
//                    lag_1 = lag_mid;
//                }else{
//                    lag_2 = lag_mid;
//                }
//            }
//            // compute difference
//            Precision change = 0;
//            for(const auto& el:system->model.elements){
//                // skip non existing elements
//                if (el == nullptr)
//                    continue;
//                change += std::abs(prev_density[el->element_id][0] -
//                                   system->getLoadCase()->element_data[SIMP_DENSITY_FACTOR][el->element_id][0]);
//            }
//
//            // end if the change is small enough
//            if(change < 0.0001){
//                break;
//            }
//
////            wasm_save(system, it+1);
//            std::cout << "Iteration " << (it+1) << " finished. Compliance: " << std::setprecision(10) << compliance << std::endl;
//        }
//
//        std::cout << "Finished computation" << std::endl;
//
//        // free the system
//        delete system;
//    }};
//    th.join();
//}
//
//int main(int argc, char* argv[]) {
//
//    Reader reader{"../resources/inputs/hook_topo.inp"};
//    System* sys = reader.read();
//    wasm_topo_opt(sys,0.9, 3,3,0.01, 0.01, 40);
//
////    ConstrainedDelaunay2D triangulate_2d{0,1,0,1,30};
////    triangulate_2d.addNode(Node{0.0,0.0});
////    triangulate_2d.addNode(Node{1.0,1.0});
////    triangulate_2d.addNode(Node{0.5,0.5});
////    std::cout << triangulate_2d << std::endl;
//}