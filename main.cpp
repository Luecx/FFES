#include "src/core/NodeData.h"
#include "src/entity/Iso2DQuad4.h"
#include "src/entity/Iso2DTri3.h"
#include "src/entity/Iso3DHex8.h"
#include "src/integration/Quadrature.h"
#include "src/material/IsotropicElasticity.h"
#include "src/matrix/DenseMatrix.h"
#include "src/matrix/Inverse.h"
#include "src/matrix/Matrix.h"
#include "src/matrix/QuickMatrix.h"
#include "src/model/Model.h"
#include "src/reader/Reader.h"
#include "src/solver/CG.h"
#include "src/system/System.h"

#include <chrono>
#include <iostream>

auto start = std::chrono::system_clock::now();

/**
 * starts the time measurement.
 * Note that this Tool is not used during search but rather for internal profilings and debugging.
 */
void startMeasure() {
    // std::cout << "starting measurement!\n";
    start = std::chrono::system_clock::now();
}

/**
 * stops the time measurement and returns the elapsed milliseconds.
 * @return
 */
int stopMeasure() {

    auto                          end  = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;

    return round(diff.count() * 1000);
    // std::cout << "measurement finished! [" << round(diff.count() * 1000) << "
    // ms]" << std::endl;
}
int main() {


    Reader reader{"../bin/test.inp"};
    auto system = reader.read();

    system->model.constraint(0,0,0,0);
    system->model.constraint(1,0,0,0);
    system->model.constraint(2,0,0,0);
    system->model.constraint(3,0,0,0);
    system->model.applyLoad(4,10,10,10);

    auto k = system->model.buildReducedStiffnessMatrix();
    auto f = system->model.buildReducedLoadVector();
    auto solution = conjugate_gradient(k, f);
    system->model.postProcessDisplacements(solution);
    std::cout << system->model.node_data[DISPLACEMENT] << std::endl;

    delete system;


//    Model model{100,100};
//    model.activateNodeSet("EINSPANNUNG");
//    ID nodeCount = 0;
//    ID elementCount = 0;
//    for(int i = 0; i < 3; i++){
//        model.setNode(nodeCount++,(float)i,0.0f,0.0f);
//        model.setNode(nodeCount++,(float)i,1.0f,0.0f);
//        model.setNode(nodeCount++,(float)i,1.0f,1.0f);
//        model.setNode(nodeCount++,(float)i,0.0f,1.0f);
//        model.activateNodeSet("NORMAL");
//    }
//
//    for(int i = 0; i < 1; i++){
//        model.setElement<Iso3DHex8>(elementCount ++,
//                                    i * 4 + 0,
//                                    i * 4 + 1,
//                                    i * 4 + 2,
//                                    i * 4 + 3,
//                                    i * 4 + 4,
//                                    i * 4 + 5,
//                                    i * 4 + 6,
//                                    i * 4 + 7);
//    }
//
//
//    model.addMaterial("mat1");
//    model.materials[model.getMaterialID("mat1")]->setElasticity<IsotropicElasticity>(210000,0.3);
//
//    model.constraint("EINSPANNUNG", 0,0,0);
//    model.applyLoad(4, 0,10,0);
//
//    model.solidSection("EALL", "mat1");
//    model.numerateUnconstrainedNodes();
//    auto mat{model.buildReducedStiffnessMatrix()};
//    auto load{model.buildReducedLoadVector()};
//    std::cout << mat << std::endl;
//
//    auto solution = conjugate_gradient(mat, load);
//
//    model.postProcessDisplacements(solution);



    return 0;
}
