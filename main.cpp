#include "src/core/NodeData.h"
#include "src/entity/Iso2DQuad4.h"
#include "src/entity/Iso2DTri3.h"
#include "src/entity/Iso3DHex8.h"
#include "src/integration/Quadrature.h"
#include "src/material/IsotropicMaterial.h"
#include "src/matrix/DenseMatrix.h"
#include "src/matrix/Inverse.h"
#include "src/matrix/ListedMatrix.h"
#include "src/matrix/Matrix.h"
#include "src/matrix/QuickMatrix.h"
#include "src/matrix/SparseMatrix.h"
#include "src/model/Model.h"
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

    Model model{400,99};

    model.activeNodeSet("EINSPANNUNG");
    for(int i = 0; i < 100; i++){
        model.addNode((float)i,0.0f,0.0f);
        model.addNode((float)i,1.0f,0.0f);
        model.addNode((float)i,1.0f,1.0f);
        model.addNode((float)i,0.0f,1.0f);
        model.activeNodeSet("NORMAL");
    }

    for(int i = 0; i < 99; i++){
        model.addElement<Iso3DHex8>(i * 4 + 0,
                                    i * 4 + 1,
                                    i * 4 + 2,
                                    i * 4 + 3,
                                    i * 4 + 4,
                                    i * 4 + 5,
                                    i * 4 + 6,
                                    i * 4 + 7);
    }


    model.addMaterial<IsotropicMaterial>("mat1", 210e9,0.0f);

    model.constraint("EINSPANNUNG", 0,0,0);

    model.applyLoad(399, 0,1000000,0);
    model.applyLoad(398, 0,1000000,0);
    model.applyLoad(397, 0,1000000,0);
    model.applyLoad(396, 0,1000000,0);

    model.solidSection("EALL", "mat1");
    model.numerateUnconstrainedNodes();
    ListedMatrix res{model.buildReducedStiffnessMatrix()};
    DenseMatrix load{model.buildReducedLoadVector(res)};
    SparseMatrix mat{res};

    auto solution =  conjugate_gradient(mat, load);
    std::cout << solution << std::endl;


//    std::cout << solution << std::endl;

//
//    std::cout << mat.getM() << std::endl;
//    QuickMatrix<108,108> dense{};
//    for(int i = 0; i < 108; i++){
//        for(int n = 0; n < 108; n++){
//            dense(i,n) = mat(i,n);
//        }
//    }
//    Precision det = 0;
//    auto inv = inverse(dense, det);
//    std::cout << "--------------------------------" << std::endl;
//    std::cout << det << std::endl;
//    std::cout << DenseMatrix(inv) * load << std::endl;


    return 0;
}
