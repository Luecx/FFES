/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFESis free software: you can redistribute it and/or modify                   *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFESis distributed in the hope that it will be useful,                     *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

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
