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
