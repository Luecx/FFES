//
// Created by Luecx on 05.09.2021.
//

#ifndef FEM_SRC_MATERIAL_MATERIAL_H_
#define FEM_SRC_MATERIAL_MATERIAL_H_

#include "../matrix/QuickMatrix.h"

struct Material {

    std::string               name;
    Precision                 density = 0;

    virtual ~Material() {}

    virtual QuickMatrix<3, 3> getMaterialMatrix2D() = 0;
    virtual QuickMatrix<6, 6> getMaterialMatrix3D() = 0;
};

#endif    // FEM_SRC_MATERIAL_MATERIAL_H_
