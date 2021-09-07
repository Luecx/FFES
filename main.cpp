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

    Model model{40,9};

    for(int i = 0; i < 10; i++){
        model.addNode(i,0,0);
        model.addNode(i,1,0);
        model.addNode(i,1,1);
        model.addNode(i,0,1);
    }

    for(int i = 0; i < 9; i++){
        model.addElement<Iso3DHex8>(i * 4 + 0,
                                    i * 4 + 1,
                                    i * 4 + 2,
                                    i * 4 + 3,
                                    i * 4 + 4,
                                    i * 4 + 5,
                                    i * 4 + 6,
                                    i * 4 + 7);
    }


    model.addMaterial<IsotropicMaterial>("mat1", 1.0,0.3);

    model.solidSection("EALL", "mat1");

    auto s1 = model.elements[0]->computeLocalStiffness();
    std::cout << s1 << std::endl;

//    startMeasure();
//    Precision f = 0;
//    for (int i = 0; i < 100000; i++) {
////        model.elements[0]->computeLocalStiffness();
//        auto s1 = model.elements[0]->computeLocalStiffness();
////        QuickMatrix<2,2> m1{};
////        m1.randomise();
////        Precision det = 0;
////        auto h = inverse2x2(m1, det);
////        f += h.at(0,0);
////        auto      h   = integrate<ISO_HEX, QUADRATIC>();
////        Precision sum = 0;
////        for (int i = 0; i < h.m; i++) {
////            sum += h(i, 3) * func(h(i, 0), h(i, 1), h(i, 2), i / 1000000.0);
////        }
////        f += sum;
//    }
//    std::cout << stopMeasure() << std::endl;
//    std::cout << f << std::endl;

//    auto stiffness = model.elements[0]->computeLocalStiffness();
//    std::cout << stiffness << std::endl;
//
//    QuickMatrix<12,12> reduced{};
//    int idx[]{3,4,5,6,7,8,15,16,17,18,19,20};
//    for(int i = 0; i < 12; i++){
//        for(int n = 0; n < 12; n++){
//            reduced(i,n) = stiffness(idx[i], idx[n]);
//        }
//    }
//
//    double det = 0;
//    auto inv = inverse(reduced, det);
//
//    std::cout << inv << std::endl;
//
//    QuickMatrix<12,1> vec{};
//    vec(0,0) = 1;
//    vec(3,0) = 1;
//    vec(6,0) = 1;
//    vec(9,0) = 1;
//
//    auto solution = inv * vec;
////    std::cout << inv * reduced << std::endl;
////    std::cout << reduced << std::endl;
////    std::cout << vec << std::endl;
//    std::cout << solution << std::endl;


//    QuickMatrix<3,3> dense_matrix1{};
//    dense_matrix1.randomise();
//
//    double v = 0;
//
//
//    auto inv = inverse(dense_matrix1, v);
//    startMeasure();
//    for(int i = 0; i < 100000; i++){
//        auto t = inverse(dense_matrix1, v);
//        inv += t;
//    }
//    std::cout << stopMeasure() << std::endl;

//    std::cout << dense_matrix1 << std::endl;
//    std::cout << inv << std::endl;
//    std::cout << inv * dense_matrix1 << std::endl;
//    std::cout << v << std::endl;

//    std::cout << permutation << std::endl;

//    QuickMatrix<3,3> dense_matrix2{};
//    dense_matrix1(2,1) = 3;
//    dense_matrix2(1,2) = 3;
//    for(int i = 0; i < 3; i++){
//        dense_matrix1(i,i) = 1;
//        dense_matrix2(i,i) = 1;
//    }
//    std::cout << dense_matrix1 << std::endl;
//    std::cout << dense_matrix2 << std::endl;
//    auto h = dense_matrix1 * dense_matrix2;
//    auto h2 = !dense_matrix1;
//    std::cout << h << std::endl;
//    std::cout << h2 << std::endl;
//    std::cout << typeid (h).name() << std::endl;
//    std::cout << sizeof(h) << std::endl;

//    ComponentContainer<float> container{};
//    container.init(1e6*3,1e6);
//    for(int i = 0; i < 1e6; i++){
//        container.setIndexIncremental(i, 4);
//    }
//
//    container[3][2] = 5;
//    std::cout << container << std::endl;



    return 0;
}
