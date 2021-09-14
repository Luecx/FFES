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

#include "IsotropicMaterial.h"
IsotropicMaterial::IsotropicMaterial(Precision p_youngs_module, Precision p_poisson)
    : youngs_module(p_youngs_module), poisson(p_poisson) {update();}
void IsotropicMaterial::update() {

    // 2d case
    Precision scalar2D = youngs_module / (1 - poisson * poisson);

    mat_matrix_2d(0,0) =  scalar2D;
    mat_matrix_2d(1,1) =  scalar2D;
    mat_matrix_2d(1,0) =  poisson * scalar2D;
    mat_matrix_2d(0,1) =  poisson * scalar2D;
    mat_matrix_2d(2,2) =  (1 - poisson) / 2 * scalar2D;


    // 3d case
    Precision scalar = youngs_module / ((1 + poisson) * (1 - 2 * poisson));

    mat_matrix_3d(0, 1)
        = mat_matrix_3d(0, 2)
        = mat_matrix_3d(1, 0)
        = mat_matrix_3d(1, 2)
        = mat_matrix_3d(2, 0)
        = mat_matrix_3d(2, 1) = poisson * scalar;

    mat_matrix_3d(0, 0)
        = mat_matrix_3d(1, 1)
        = mat_matrix_3d(2, 2) = (1 - poisson) * scalar;


    mat_matrix_3d(3, 3)
        = mat_matrix_3d(4, 4)
        = mat_matrix_3d(5, 5) = (1 - 2 * poisson) * scalar;
}
QuickMatrix<3, 3> IsotropicMaterial::getMaterialMatrix2D() { return mat_matrix_2d; }
QuickMatrix<6, 6> IsotropicMaterial::getMaterialMatrix3D() { return mat_matrix_3d; }
