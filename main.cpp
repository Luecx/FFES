#include "src/core/NodeData.h"
#include "src/entity/Iso2DQuad4.h"
#include "src/entity/Iso2DTri3.h"
#include "src/entity/Iso3DHex8.h"
#include "src/integration/Quadrature.h"
#include "src/material/IsotropicMaterial.h"
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

//    Model model{8,1};
//
//    model.activeNodeSet("EINSPANNUNG");
//    for(int i = 0; i < 2; i++){
//        model.addNode((float)i,0.0f,0.0f);
//        model.addNode((float)i,1.0f,0.0f);
//        model.addNode((float)i,1.0f,1.0f);
//        model.addNode((float)i,0.0f,1.0f);
//        model.activeNodeSet("NORMAL");
//    }
//
//    for(int i = 0; i < 1; i++){
//        model.addElement<Iso3DHex8>(i * 4 + 0,
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
//    model.addMaterial<IsotropicMaterial>("mat1", 210e9,0.0f);
//
//    model.constraint("EINSPANNUNG", 0,0,0);
//    model.constraint(7, 0.1,NONE,NONE);
//
//    model.solidSection("EALL", "mat1");
//    model.numerateUnconstrainedNodes();
//    auto mat{model.buildReducedStiffnessMatrix()};
//    auto load{model.buildReducedLoadVector()};
//    std::cout << load << std::endl;
//    std::cout << mat << std::endl;
//    auto solution = conjugate_gradient(mat, load);
//
//    model.postProcessDisplacements(solution);
//    std::cout << model.node_data[DISPLACEMENT] << std::endl;

    Reader reader{};
    reader.read("../test.inp");


    return 0;
}
