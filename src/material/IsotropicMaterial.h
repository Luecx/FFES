//
// Created by Luecx on 05.09.2021.
//

#ifndef FEM_SRC_MATERIAL_ISOTROPICMATERIAL_H_
#define FEM_SRC_MATERIAL_ISOTROPICMATERIAL_H_

#include "Material.h"

struct IsotropicMaterial : Material {

    private:

    Precision youngs_module;
    Precision poisson;

    QuickMatrix<3,3> mat_matrix_2d{};
    QuickMatrix<6,6> mat_matrix_3d{};

    public:
    IsotropicMaterial(Precision youngs_module, Precision poisson);

    private:
    void update();

    public:
    QuickMatrix<3, 3> getMaterialMatrix2D() override;
    QuickMatrix<6, 6> getMaterialMatrix3D() override;

};

#endif    // FEM_SRC_MATERIAL_ISOTROPICMATERIAL_H_
