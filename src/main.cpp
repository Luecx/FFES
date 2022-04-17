#include "core/NodeData.h"
#include "entity/Iso2DQuad4.h"
#include "entity/Iso2DQuad8.h"
#include "entity/Iso2DTri3.h"
#include "entity/Iso2DTri6.h"
#include "entity/Iso3DHex20.h"
#include "entity/Iso3DHex8.h"
#include "entity/IsoElement.h"
#include "material/IsotropicElasticity.h"
#include "model/Model.h"
#include "reader/Reader.h"
#include "delauney/Node.h"
#include "delauney/Edge.h"
#include "delauney/Triangle.h"
#include "delauney/Triangulate2D.h"
#include <random>
#include <thread>

#include <iostream>



//void testC2D8(){
//        // folding for C2D8
//        System system{50,5};
//        int element_count = 3;
//        Precision element_size  = 10;
//        int node_id = 0;
//        for(int i = 0; i < element_count+1; i++){
//            system.model.setNode(node_id ++, i * element_size                 , -element_size/2);
//            system.model.setNode(node_id ++, i * element_size + element_size/2, -element_size/2);
//            system.model.setNode(node_id ++, i * element_size + element_size/2, +element_size/2);
//            system.model.setNode(node_id ++, i * element_size                 , +element_size/2);
//            system.model.setNode(node_id ++, i * element_size                 , 0);
//        }
//        std::cout << system.model.node_data[POSITION] << std::endl;
//        for(int i = 0; i < element_count; i++){
//            int index_1 = i * 5;
//            int index_2 = (i+1) * 5;
//            system.model.setElement<Iso2DQuad8>(i,
//                                                index_1,
//                                                index_2,
//                                                index_2+3,
//                                                index_1+3,
//                                                index_1+1,
//                                                index_2+4,
//                                                index_1+2,
//                                                index_1+4);
//        }
//        // constraint left nodes
//        system.getLoadCase()->constraint(0,0,0,0,false);
//        system.getLoadCase()->constraint(3,0,0,0,false);
//        system.getLoadCase()->constraint(4,0,0,0,false);
//        // load right top? node
//        system.getLoadCase()->applyLoad(element_count*5+3,1,10);
//        // create section
//        system.model.addMaterial("mat1");
//        system.model.materials[0].setElasticity<IsotropicElasticity>(70000,0);
//        system.model.solidSection("EALL", "mat1", 1);
//
//        system.getLoadCase()->compute();
//
//        for(int i = 0; i < system.model.max_node_count;i++){
//            if(system.model.node_data[NODE_CONNECTED_ELEMENTS][i][0] == 0) continue;
//            std::cout << system.model.node_data[POSITION][i][0] << ","
//                      << system.model.node_data[POSITION][i][1] << ","
//                      << system.model.node_data[POSITION][i][2] << ",";
//            std::cout << system.getLoadCase()->node_data[DISPLACEMENT][i][0] << ","
//                      << system.getLoadCase()->node_data[DISPLACEMENT][i][1] << ","
//                      << system.getLoadCase()->node_data[DISPLACEMENT][i][2] << ",";
//            std::cout << system.getLoadCase()->node_data[NODAL_STRESS][i][0] << ","
//                      << system.getLoadCase()->node_data[NODAL_STRESS][i][1] << ","
//                      << system.getLoadCase()->node_data[NODAL_STRESS][i][2] << "\n";
//        }
//}
//
//void testC3D20(){
//    System system{2000,100};
//
//        // folding for C3D20
//
//        int element_count = 6;
//        int element_size  = 2;
//
//        auto half = element_size / 2;
//
//        int node_id = 0;
//        for(int i = 0; i < 100; i++){
//            // create nodes
//            system.model.setNode(node_id++,-(-half) , -half,(i * element_size       ) );
//            system.model.setNode(node_id++,-(-half) ,-half ,(i * element_size + half) );
//            system.model.setNode(node_id++,-(-half) , half ,(i * element_size + half) );
//            system.model.setNode(node_id++,-(-half) ,  half,(i * element_size       ) );
//            system.model.setNode(node_id++,-(-half) ,   0.0,(i * element_size       ) );
//            system.model.setNode(node_id++,-( half) , -half,(i * element_size       ) );
//            system.model.setNode(node_id++,-( half) ,-half ,(i * element_size + half) );
//            system.model.setNode(node_id++,-( half) , half ,(i * element_size + half) );
//            system.model.setNode(node_id++,-( half) ,  half,(i * element_size       ) );
//            system.model.setNode(node_id++,-( half) ,   0.0,(i * element_size       ) );
//            system.model.setNode(node_id++,-(  0.0) , -half,(i * element_size       ) );
//            system.model.setNode(node_id++,-(  0.0) ,  half,(i * element_size       ) );
//        }
//
//        for(int j = 0; j < element_count; j++) {
//            int first_index  =  j      * 12;
//            int second_index = (j + 1) * 12;
//            system.model.setElement<Iso3DHex20>(
//                j,
//                first_index,
//                second_index,
//                second_index + 3,
//                first_index + 3,
//                first_index + 5,
//                second_index + 5,
//                second_index + 8,
//                first_index + 8,
//
//                first_index + 1,
//                second_index + 4,
//                first_index + 2,
//                first_index + 4,
//
//                first_index + 6,
//                second_index + 9,
//                first_index + 7,
//                first_index + 9,
//
//                first_index + 10,
//                second_index + 10,
//                second_index + 11,
//                first_index + 11);
//        }
//
//        system.getLoadCase()->constraint(0,0,0,0,true);
//        system.getLoadCase()->constraint(3,0,0,0,true);
//        system.getLoadCase()->constraint(4,0,0,0,true);
//        system.getLoadCase()->constraint(5,0,0,0,true);
//        system.getLoadCase()->constraint(8,0,0,0,true);
//        system.getLoadCase()->constraint(9,0,0,0,true);
//        system.getLoadCase()->constraint(10,0,0,0,true);
//        system.getLoadCase()->constraint(11,0,0,0, true);
//
//        // twisting
//    //    system.getLoadCase()->applyLoad(element_count * 12 + 0 ,0,1,false);
//    //    system.getLoadCase()->applyLoad(element_count * 12 + 3 ,1,1,false);
//    //    system.getLoadCase()->applyLoad(element_count * 12 + 5 ,1,-1,false);
//    //    system.getLoadCase()->applyLoad(element_count * 12 + 8 ,0,-1,false);
//
//        // bending
//        system.getLoadCase()->applyLoad(element_count * 12 + 0 ,0,1,false);
//        system.getLoadCase()->applyLoad(element_count * 12 + 3 ,0,1,false);
//        system.getLoadCase()->applyLoad(element_count * 12 + 5 ,0,1,false);
//        system.getLoadCase()->applyLoad(element_count * 12 + 8 ,0,1,false);
//
//    //    system.getLoadCase()->applyLoad(element_count * 12 + 4 ,0,10* 1.0/4 ,false);
//    //    system.getLoadCase()->applyLoad(element_count * 12 + 11,0,10* 1.0/4 ,false);
//    //    system.getLoadCase()->applyLoad(element_count * 12 + 9 ,0,10* 1.0/4 ,false);
//    //    system.getLoadCase()->applyLoad(element_count * 12 + 10,0,10* 1.0/4 ,false);
//
//}
//
//void testShapeFunction() {
//    Iso3DHex8   quad {1, 2, 3, 4, 5, 6, 7, 8};
//
//    // check shape function
//    auto        local_coords = quad.getNodeLocalCoordinates();
//    DenseMatrix node_shape_values {quad.nodeCount(), quad.nodeCount()};
//    for (int i = 0; i < quad.nodeCount(); i++) {
//        auto r     = local_coords(i, 0);
//        auto s     = local_coords(i, 1);
//        auto t     = local_coords(i, 2);
//        auto shape = quad.getShapeFunction(r, s, t);
//        for (int j = 0; j < quad.nodeCount(); j++) {
//            node_shape_values(i, j) = shape(j, 0);
//        }
//    }
//    std::cout << "------------------------------------------------------------------" << std::endl;
//    std::cout << "has to be identity matrix:" << std::endl;
//    std::cout << node_shape_values << std::endl;
//
//    // verify derivative
//    Precision delta = 0.001;
//    for (Precision r = -1; r < 1; r += 0.4) {
//        for (Precision s = -1; s < 1; s += 0.4) {
//            for (Precision t = -1; t < 1; t += 0.4) {
//                auto derivative    = quad.getLocalShapeDerivative(r, s, t);
//
//                auto shape_func_rp = quad.getShapeFunction(r + delta, s, t);
//                auto shape_func_rm = quad.getShapeFunction(r - delta, s, t);
//                auto shape_func_sp = quad.getShapeFunction(r, s + delta, t);
//                auto shape_func_sm = quad.getShapeFunction(r, s - delta, t);
//                auto shape_func_tp = quad.getShapeFunction(r, s, t + delta);
//                auto shape_func_tm = quad.getShapeFunction(r, s, t - delta);
//                for (int node = 0; node < quad.nodeCount(); node++) {
//                    Precision grad_r =
//                        (shape_func_rp(node, 0) - shape_func_rm(node, 0)) / (2 * delta);
//                    Precision grad_s =
//                        (shape_func_sp(node, 0) - shape_func_sm(node, 0)) / (2 * delta);
//                    Precision grad_t =
//                        (shape_func_tp(node, 0) - shape_func_tm(node, 0)) / (2 * delta);
//                    if (std::abs(grad_r - derivative(0, node)) > 0.0001) {
//                        std::cout << grad_r << "  " << derivative(0, node) << std::endl;
//                    }
//                    if (std::abs(grad_s - derivative(1, node)) > 0.0001) {
//                        std::cout << grad_s << "  " << derivative(1, node) << std::endl;
//                    }
//                    if (std::abs(grad_t - derivative(2, node)) > 0.0001) {
//                        std::cout << grad_t << "  " << derivative(2, node) << std::endl;
//                    }
//                }
//            }
//    }
//}

template <
    class result_t   = std::chrono::microseconds,
    class clock_t    = std::chrono::steady_clock,
    class duration_t = std::chrono::microseconds
    >
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

int main(int argc, char* argv[]) {

    using namespace delaunay;

//    Node n1{0.303995,0.0149846};
//    Node n2{4,-1};
//    Node n3{0.585009,0.479873};
//    Node n4{0.710501,0.513535};
//
//    Triangle t1{};
//    Triangle t2{};
//    t1.createEdges(&n1,&n2,&n3);
//    t2.createEdges(&n2,&n4,&n3);
//    t1.getE2().link(&t2.e3);
//
//    std::cout << t1.imscribed(&n4) << std::endl;

    int tests[]{100000};

    for(int& h:tests){
        Triangulate2D triangulate_2d{0,1,0,1,h};
        std::vector<Node> nodes{};
        for(int i = 0; i < h; i++){
            float x = (i % (int)sqrt(h)) / (float)(int)sqrt(h);
            float y = (i / (int)sqrt(h)) / (float)(int)sqrt(h);
//            float x = rand() / (Precision)(RAND_MAX);
//            float y = rand() / (Precision)(RAND_MAX);
            nodes.push_back({x,y});
        }
        std::shuffle(nodes.begin(), nodes.end(), std::mt19937(std::random_device()()));
        auto start = std::chrono::steady_clock::now();
//        std::srand(start.time_since_epoch().count());
        for(int i = 0; i < h; i++){
            triangulate_2d.addNode(nodes[i]);
        }
        std::cout << "Elapsed(mms)=" << since(start).count() << std::endl;
//        std::cout << triangulate_2d << std::endl;
    }
}