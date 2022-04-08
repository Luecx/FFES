
//
// Created by Luecx on 15.09.2021.
//

#ifndef FEM_SRC_MATERIAL_ELASTICITY_H_
#define FEM_SRC_MATERIAL_ELASTICITY_H_

#include "../matrix/QuickMatrix.h"
#include <memory>


struct Elasticity{

    QuickMatrix<3,3> mat_matrix_2d{};
    QuickMatrix<6,6> mat_matrix_3d{};

    virtual void update() = 0;

    virtual ~Elasticity(){}
    QuickMatrix<3, 3> getMaterialMatrix2D() {
        update();
        return mat_matrix_2d;
    }
    QuickMatrix<6, 6> getMaterialMatrix3D() {
        update();
        return mat_matrix_3d;
    }
};

using ElasticityPtr = std::shared_ptr<Elasticity>;

#endif    // FEM_SRC_MATERIAL_ELASTICITY_H_
