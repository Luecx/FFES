#include "src/core/NodeData.h"
#include "src/entity/Iso2DQuad4.h"
#include "src/entity/Iso3DHex8.h"
#include "src/material/IsotropicElasticity.h"
#include "src/model/Model.h"
#include "src/reader/Reader.h"
#include "src/entity/IsoElement.h"

#include <iostream>

int main(int argc, char* argv[]) {


    Reader reader{argv[1]};
    auto system = reader.read();
    system->getLoadCase()->compute();


    std::ofstream myfile;
    myfile.open (argv[2]);

    int element_id = 1;
    for(auto el:system->model.elements){
        if(el == nullptr) continue;

        auto comp   = el->compliance(system->getLoadCase());

        myfile << element_id ++ << ",";
        myfile << comp;
        myfile << "\n";
    }

    delete system;
    myfile.close();

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
//            disp(0,0) = system->getLoadCase()->node_data[DISPLACEMENT][el->nodeIDS()[i]][0];
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
