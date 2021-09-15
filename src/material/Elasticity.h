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
//
// Created by Luecx on 15.09.2021.
//

#ifndef FEM_SRC_MATERIAL_ELASTICITY_H_
#define FEM_SRC_MATERIAL_ELASTICITY_H_

#include "../matrix/QuickMatrix.h"

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

#endif    // FEM_SRC_MATERIAL_ELASTICITY_H_
