#include "src/core/NodeData.h"
#include "src/entity/Iso2DQuad4.h"
#include "src/entity/Iso3DHex20.h"
#include "src/entity/Iso3DHex8.h"
#include "src/entity/IsoElement.h"
#include "src/material/IsotropicElasticity.h"
#include "src/model/Model.h"
#include "src/reader/Reader.h"

#include <iostream>

int main(int argc, char* argv[]) {


    System system{2000,10};

    int node_id = 0;


    int element_count = 10;
    int element_size  = 10;
    
    auto half = element_size / 2;
    
    for(int i = 0; i < 100; i++){
        // create nodes
        system.model.setNode(node_id++, i * element_size      , -half / 10.0, -half);
        system.model.setNode(node_id++, i * element_size + half,-half / 10.0, -half);
        system.model.setNode(node_id++, i * element_size + half, half / 10.0, -half);
        system.model.setNode(node_id++, i * element_size      ,  half / 10.0, -half);
        system.model.setNode(node_id++, i * element_size      ,   0.0 / 10.0, -half);
        system.model.setNode(node_id++, i * element_size      , -half / 10.0,  half);
        system.model.setNode(node_id++, i * element_size + half,-half / 10.0,  half);
        system.model.setNode(node_id++, i * element_size + half, half / 10.0,  half);
        system.model.setNode(node_id++, i * element_size      ,  half / 10.0,  half);
        system.model.setNode(node_id++, i * element_size      ,   0.0 / 10.0,  half);
        system.model.setNode(node_id++, i * element_size      , -half / 10.0,   0.0);
        system.model.setNode(node_id++, i * element_size      ,  half / 10.0,   0.0);
    }
//    system.model.setNode(0 ,-1,-1,-1);
//    system.model.setNode(1 , 1,-1,-1);
//    system.model.setNode(2 , 1, 1,-1);
//    system.model.setNode(3 ,-1, 1,-1);
//    system.model.setNode(4 ,-1,-1, 1);
//    system.model.setNode(5 , 1,-1, 1);
//    system.model.setNode(6 , 1, 1, 1);
//    system.model.setNode(7 ,-1, 1, 1);
//
//    system.model.setNode(8 , 0,-1,-1);
//    system.model.setNode(9 , 1, 0,-1);
//    system.model.setNode(10, 0, 1,-1);
//    system.model.setNode(11,-1, 0,-1);
//
//    system.model.setNode(12, 0,-1, 1);
//    system.model.setNode(13, 1, 0, 1);
//    system.model.setNode(14, 0, 1, 1);
//    system.model.setNode(15,-1, 0, 1);
//
//    system.model.setNode(16,-1,-1, 0);
//    system.model.setNode(17, 1,-1, 0);
//    system.model.setNode(18, 1, 1, 0);
//    system.model.setNode(19,-1, 1, 0);
//
//    system.model.setElement<Iso3DHex20>(0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19);
////    system.model.setElement<Iso3DHex8>(0,0,1,2,3,4,5,6,7);
//
//    system.getLoadCase()->constraint(0 ,0,0,0,true);
//    system.getLoadCase()->constraint(3 ,0,0,0,true);
//    system.getLoadCase()->constraint(4 ,0,0,0,true);
//    system.getLoadCase()->constraint(7 ,0,0,0,true);
//    system.getLoadCase()->constraint(11,0,0,0,true);
//    system.getLoadCase()->constraint(15,0,0,0,true);
//    system.getLoadCase()->constraint(16,0,0,0,true);
//    system.getLoadCase()->constraint(19,0,0,0,true);
//
//    system.getLoadCase()->applyLoad(1 ,0, 1.0,false);
//    system.getLoadCase()->applyLoad(2 ,0, 1.0,false);
//    system.getLoadCase()->applyLoad(5 ,0, 1.0,false);
//    system.getLoadCase()->applyLoad(6 ,0, 1.0,false);
//    system.getLoadCase()->applyLoad(9 ,0, 1.0,false);
//    system.getLoadCase()->applyLoad(13,0, 1.0,false);
//    system.getLoadCase()->applyLoad(17,0, 1.0,false);
//    system.getLoadCase()->applyLoad(18,0, 1.0,false);
//
//    system.model.addMaterial("mat1");
//    system.model.materials[0]->setElasticity<IsotropicElasticity>(1,0);
//    system.model.solidSection("EALL", "mat1");
//
//    system.getLoadCase()->compute();
//
//    std::cout << system.getLoadCase()->node_data[DISPLACEMENT];


    for(int j = 0; j < element_count; j++) {
        int first_index  =  j      * 12;
        int second_index = (j + 1) * 12;
        system.model.setElement<Iso3DHex20>(
            j,
            first_index,
            second_index,
            second_index + 3,
            first_index + 3,
            first_index + 5,
            second_index + 5,
            second_index + 8,
            first_index + 8,

            first_index + 1,
            second_index + 4,
            first_index + 2,
            first_index + 4,

            first_index + 6,
            second_index + 9,
            first_index + 7,
            first_index + 9,

            first_index + 10,
            second_index + 10,
            second_index + 11,
            first_index + 11);
    }
//
//    for(int i = 0; i < system.model.elements[0]->nodeCount(); i++){
//        std::cout << system.model.node_data[POSITION][system.model.elements[0]->nodeIDS()[i]][0]
//             << "  " << system.model.node_data[POSITION][system.model.elements[0]->nodeIDS()[i]][1]
//             << "  " << system.model.node_data[POSITION][system.model.elements[0]->nodeIDS()[i]][2] << std::endl;
//    }
//
    system.getLoadCase()->constraint(0,0,0,0,true);
    system.getLoadCase()->constraint(3,0,0,0,true);
    system.getLoadCase()->constraint(4,0,0,0,true);
    system.getLoadCase()->constraint(5,0,0,0,true);
    system.getLoadCase()->constraint(8,0,0,0,true);
    system.getLoadCase()->constraint(9,0,0,0,true);
    system.getLoadCase()->constraint(10,0,0,0,true);
    system.getLoadCase()->constraint(11,0,0,0, true);

//    for(int i = 0; i < 24; i++){
//
//        system.getLoadCase()->applyLoad(i,0,-1.0,false);
//    }
//
//    system.getLoadCase()->applyLoad(element_count * 12 + 0 ,2,10*-1.0/12,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 3 ,2,10*-1.0/12,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 8 ,2,10*-1.0/12,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 5 ,2,10*-1.0/12,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 4 ,2,10* 1.0/3 ,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 11,2,10* 1.0/3 ,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 9 ,2,10* 1.0/3 ,false);
//    system.getLoadCase()->applyLoad(element_count * 12 + 10,2,10* 1.0/3 ,false);

    system.getLoadCase()->applyLoad(element_count * 12 + 4 ,2,10* 1.0/4 ,false);
    system.getLoadCase()->applyLoad(element_count * 12 + 11,2,10* 1.0/4 ,false);
    system.getLoadCase()->applyLoad(element_count * 12 + 9 ,2,10* 1.0/4 ,false);
    system.getLoadCase()->applyLoad(element_count * 12 + 10,2,10* 1.0/4 ,false);

    system.model.addMaterial("mat1");
    system.model.materials[0]->setElasticity<IsotropicElasticity>(70000,0.1);
    system.model.solidSection("EALL", "mat1");

    system.getLoadCase()->compute();

    auto first = dynamic_cast<Iso3DHex20*>(system.model.elements[0]);
    std::cout << first->computeStress(system.getLoadCase(), DenseMatrix(1,3,-1.0,-1.0,-1.0));


    std::ofstream myfile;
    myfile.open ("test.out");
    for(int i = 0; i < system.model.max_node_count;i++){
        if(system.model.node_data[NODE_CONNECTED_ELEMENTS][i][0] == 0) continue;
        myfile << system.model.node_data[POSITION][i][0] << ","
               << system.model.node_data[POSITION][i][1] << ","
               << system.model.node_data[POSITION][i][2] << ",";
        myfile << system.getLoadCase()->node_data[DISPLACEMENT][i][0] << ","
               << system.getLoadCase()->node_data[DISPLACEMENT][i][1] << ","
               << system.getLoadCase()->node_data[DISPLACEMENT][i][2] << "\n";
    }
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

//
//    QuickMatrix<3,3> matrix;
//    matrix.reshape<9,1>();
//
//    std::ofstream myfile;
//    myfile.open (argv[2]);
//
//    int element_id = 1;
//    for(auto el:system->model.elements){
//        if(el == nullptr) continue;
//
//        auto stress = el->computeStress(system->getLoadCase(), el->getIntegrationScheme());
//        QuickMatrix<3,1> disp_sum{};
//        QuickMatrix<6,1> stress_sum{};
//        for(int i = 0; i < el->nodeCount(); i++){
//            QuickMatrix<3,1> disp{};
//            disp(-1.0) = system->getLoadCase()->node_data[DISPLACEMENT][el->nodeIDS()[i]][0];
//            disp(1,0) = system->getLoadCase()->node_data[DISPLACEMENT][el->nodeIDS()[i]][1];
//            disp(2,0) = system->getLoadCase()->node_data[DISPLACEMENT][el->nodeIDS()[i]][2];
//            disp_sum += disp;
//            QuickMatrix<6,1> str{};
//            for(int j = 0; j < 6; j++){
//                str(j,0) = stress(j,i);
//            }
//            stress_sum += str;
//        }
//        disp_sum   *= 0.125;
//        stress_sum *= 0.125;
//        myfile << element_id ++ << ",";
//        for(int i = 0; i < 3; i++){
//            myfile << disp_sum(i,0) << ", ";
//        }
//        for(int i = 0; i < 6; i++){
//            myfile << stress_sum(i,0) << ", ";
//        }
//        myfile << "\n";
//    }
//
//    delete system;
//    myfile.close();


    return 0;
}
