#include "src/core/NodeData.h"
#include "src/entity/Iso2DQuad4.h"
#include "src/entity/Iso2DQuad8.h"
#include "src/entity/Iso2DTri3.h"
#include "src/entity/Iso2DTri6.h"
#include "src/entity/Iso3DHex20.h"
#include "src/entity/Iso3DHex8.h"
#include "src/entity/IsoElement.h"
#include "src/material/IsotropicElasticity.h"
#include "src/model/Model.h"
#include "src/reader/Reader.h"

#include <iostream>

int main(int argc, char* argv[]) {


//    System system{2000,100};
//
//    // folding for C3D20
//
//    int element_count = 6;
//    int element_size  = 2;
//
//    auto half = element_size / 2;
//
//    int node_id = 0;
//    for(int i = 0; i < 100; i++){
//        // create nodes
//        system.model.setNode(node_id++,-(-half) , -half,(i * element_size       ) );
//        system.model.setNode(node_id++,-(-half) ,-half ,(i * element_size + half) );
//        system.model.setNode(node_id++,-(-half) , half ,(i * element_size + half) );
//        system.model.setNode(node_id++,-(-half) ,  half,(i * element_size       ) );
//        system.model.setNode(node_id++,-(-half) ,   0.0,(i * element_size       ) );
//        system.model.setNode(node_id++,-( half) , -half,(i * element_size       ) );
//        system.model.setNode(node_id++,-( half) ,-half ,(i * element_size + half) );
//        system.model.setNode(node_id++,-( half) , half ,(i * element_size + half) );
//        system.model.setNode(node_id++,-( half) ,  half,(i * element_size       ) );
//        system.model.setNode(node_id++,-( half) ,   0.0,(i * element_size       ) );
//        system.model.setNode(node_id++,-(  0.0) , -half,(i * element_size       ) );
//        system.model.setNode(node_id++,-(  0.0) ,  half,(i * element_size       ) );
//    }
//
//    for(int j = 0; j < element_count; j++) {
//        int first_index  =  j      * 12;
//        int second_index = (j + 1) * 12;
//        system.model.setElement<Iso3DHex20>(
//            j,
//            first_index,
//            second_index,
//            second_index + 3,
//            first_index + 3,
//            first_index + 5,
//            second_index + 5,
//            second_index + 8,
//            first_index + 8,
//
//            first_index + 1,
//            second_index + 4,
//            first_index + 2,
//            first_index + 4,
//
//            first_index + 6,
//            second_index + 9,
//            first_index + 7,
//            first_index + 9,
//
//            first_index + 10,
//            second_index + 10,
//            second_index + 11,
//            first_index + 11);
//    }
//
//    system.getLoadCase()->constraint(0,0,0,0,true);
//    system.getLoadCase()->constraint(3,0,0,0,true);
//    system.getLoadCase()->constraint(4,0,0,0,true);
//    system.getLoadCase()->constraint(5,0,0,0,true);
//    system.getLoadCase()->constraint(8,0,0,0,true);
//    system.getLoadCase()->constraint(9,0,0,0,true);
//    system.getLoadCase()->constraint(10,0,0,0,true);
//    system.getLoadCase()->constraint(11,0,0,0, true);
//
//    // twisting
////    system.getLoadCase()->applyLoad(element_count * 12 + 0 ,0,1,false);
////    system.getLoadCase()->applyLoad(element_count * 12 + 3 ,1,1,false);
////    system.getLoadCase()->applyLoad(element_count * 12 + 5 ,1,-1,false);
////    system.getLoadCase()->applyLoad(element_count * 12 + 8 ,0,-1,false);
//
//    // bending
//    system.getLoadCase()->applyLoad(element_count * 12 + 0 ,0,1,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 3 ,0,1,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 5 ,0,1,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 8 ,0,1,false);
//
////    system.getLoadCase()->applyLoad(element_count * 12 + 4 ,0,10* 1.0/4 ,false);
////    system.getLoadCase()->applyLoad(element_count * 12 + 11,0,10* 1.0/4 ,false);
////    system.getLoadCase()->applyLoad(element_count * 12 + 9 ,0,10* 1.0/4 ,false);
////    system.getLoadCase()->applyLoad(element_count * 12 + 10,0,10* 1.0/4 ,false);
//
//    // folding for C2D8
//    int element_count = 20;
//    Precision element_size  = 10;
//    int node_id = 0;
//    for(int i = 0; i < element_count+1; i++){
//        system.model.setNode(node_id ++, i * element_size                 , -element_size/2);
//        system.model.setNode(node_id ++, i * element_size + element_size/2, -element_size/2);
//        system.model.setNode(node_id ++, i * element_size + element_size/2, +element_size/2);
//        system.model.setNode(node_id ++, i * element_size                 , +element_size/2);
//        system.model.setNode(node_id ++, i * element_size                 , 0);
//    }
//    for(int i = 0; i < element_count; i++){
//        int index_1 = i * 5;
//        int index_2 = (i+1) * 5;
//        system.model.setElement<Iso2DQuad8>(i,index_1,index_2,index_2+3, index_1+3, index_1+1, index_2+4, index_1+2, index_1+4);
//    }
//    // constraint left nodes
//    system.getLoadCase()->constraint(0,0,0,0,false);
//    system.getLoadCase()->constraint(3,0,0,0,false);
//    system.getLoadCase()->constraint(4,0,0,0,false);
//    // load right bottom node
//    system.getLoadCase()->applyLoad(element_count*5+3,1,10);
//    // create section
//
//
//    system.model.addMaterial("mat1");
//    system.model.materials[0]->setElasticity<IsotropicElasticity>(70000,0);
//    system.model.solidSection("EALL", "mat1", 1);
//
//    system.getLoadCase()->compute();
//
//    std::ofstream myfile;
//    myfile.open ("test.out");
//    for(int i = 0; i < system.model.max_node_count;i++){
//        if(system.model.node_data[NODE_CONNECTED_ELEMENTS][i][0] == 0) continue;
//        myfile << system.model.node_data[POSITION][i][0] << ","
//               << system.model.node_data[POSITION][i][1] << ","
//               << system.model.node_data[POSITION][i][2] << ",";
//        myfile << system.getLoadCase()->node_data[DISPLACEMENT][i][0] << ","
//               << system.getLoadCase()->node_data[DISPLACEMENT][i][1] << ","
//               << system.getLoadCase()->node_data[DISPLACEMENT][i][2] << "\n";
//    }
//    myfile.close();



//    Iso3DHex8 quad{1,2,3,4,5,6,7,8};
//
//    // check shape function
//    auto local_coords = quad.getNodeLocalCoordinates();
//    DenseMatrix node_shape_values{quad.nodeCount(), quad.nodeCount()};
//    for(int i = 0; i < quad.nodeCount(); i++){
//        auto r = local_coords(i,0);
//        auto s = local_coords(i,1);
//        auto t = local_coords(i,2);
//        auto shape = quad.getShapeFunction(r,s,t);
//        for(int j = 0; j < quad.nodeCount(); j++){
//            node_shape_values(i,j) = shape(j,0);
//        }
//    }
//    std::cout << "------------------------------------------------------------------" << std::endl;
//    std::cout << "has to be identity matrix:" << std::endl;
//    std::cout << node_shape_values << std::endl;
//
//    // verify derivative
//    Precision delta = 0.001;
//    for(Precision r = -1; r < 1; r+= 0.4){
//        for(Precision s = -1; s < 1; s+= 0.4){
//            for(Precision t = -1; t < 1; t+= 0.4){
//                auto derivative = quad.getLocalShapeDerivative(r,s,t);
//
//                auto shape_func_rp = quad.getShapeFunction(r+delta,s,t);
//                auto shape_func_rm = quad.getShapeFunction(r-delta,s,t);
//                auto shape_func_sp = quad.getShapeFunction(r,s+delta,t);
//                auto shape_func_sm = quad.getShapeFunction(r,s-delta,t);
//                auto shape_func_tp = quad.getShapeFunction(r,s,t+delta);
//                auto shape_func_tm = quad.getShapeFunction(r,s,t-delta);
//                for(int node = 0; node < quad.nodeCount(); node ++){
//                    Precision grad_r = (shape_func_rp(node,0) - shape_func_rm(node,0)) / (2 * delta);
//                    Precision grad_s = (shape_func_sp(node,0) - shape_func_sm(node,0)) / (2 * delta);
//                    Precision grad_t = (shape_func_tp(node,0) - shape_func_tm(node,0)) / (2 * delta);
//                    if(std::abs(grad_r - derivative(0,node)) > 0.0001){
//                        std::cout << grad_r << "  " << derivative(0,node) << std::endl;
//                    }if(std::abs(grad_s - derivative(1,node)) > 0.0001){
//                        std::cout << grad_s << "  " << derivative(1,node) << std::endl;
//                    }if(std::abs(grad_t - derivative(2,node)) > 0.0001){
//                        std::cout << grad_t << "  " << derivative(2,node) << std::endl;
//                    }
//                }
//            }
//
//        }
//    }

    Reader reader{argv[1]};
    auto system = reader.read();
    system->getLoadCase()->compute();

    std::ofstream myfile;
    myfile.open (argv[2]);

    for(int i = 0; i < system->model.max_node_count; i++){
        if(system->model.node_data[NODE_CONNECTED_ELEMENTS][i][0] == 0) continue;
        myfile
            << std::setw(16) << system->model.node_data[POSITION][i][0]
            << std::setw(16) << system->model.node_data[POSITION][i][1]
            << std::setw(16) << system->getLoadCase()->node_data[DISPLACEMENT][i][0]
            << std::setw(16) << system->getLoadCase()->node_data[DISPLACEMENT][i][1]
            << std::setw(16) << system->getLoadCase()->node_data[NODAL_STRESS][i][0]
            << std::setw(16) << system->getLoadCase()->node_data[NODAL_STRESS][i][1]
            << std::setw(16) << system->getLoadCase()->node_data[NODAL_STRESS][i][2] << std::endl;
    }

    delete system;
    myfile.close();

//    Reader reader{argv[1]};
//    auto system = reader.read();
//    system->getLoadCase()->compute();
//
//
//    std::ofstream myfile;
//    myfile.open (argv[2]);
//
//    int element_id = 1;
//    for(auto el:system->model.elements){
//        if(el == nullptr) continue;
//
//        auto comp   = el->compliance(system->getLoadCase());
//
//        myfile << element_id ++ << ",";
//        myfile << comp;
//        myfile << "\n";
//    }
//
//    delete system;
//    myfile.close();


    return 0;
}
