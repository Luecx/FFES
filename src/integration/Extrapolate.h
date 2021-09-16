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

#ifndef FEM_SRC_INTEGRATION_EXTRAPOLATE_H_
#define FEM_SRC_INTEGRATION_EXTRAPOLATE_H_

#include "Quadrature.h"

template<Domain domain, Order order>
DenseMatrix extrapolate(){
    if constexpr (domain == ISO_TRI){

        if constexpr (order == LINEAR){
            return DenseMatrix{3,1,
                1,
                1,
                1};
        }

        if constexpr (order == QUADRATIC){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }

        if constexpr (order == CUBIC){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }
    }

    if constexpr (domain == ISO_QUAD){

        if constexpr (order == LINEAR){
            return DenseMatrix{4,4,
                1 + 0.5 * sqrt(3),   - 0.5          , 1 - 0.5 * sqrt(3),   - 0.5,
                  - 0.5          , 1 + 0.5 * sqrt(3),   - 0.5          , 1 - 0.5 * sqrt(3),
                1 - 0.5 * sqrt(3),   - 0.5          , 1 + 0.5 * sqrt(3),   - 0.5,
                  - 0.5          , 1 - 0.5 * sqrt(3),   - 0.5          , 1 + 0.5 * sqrt(3)};
        }

        if constexpr (order == QUADRATIC){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }
    }


    if constexpr (domain == ISO_HEX){

        if constexpr (order == LINEAR){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }

        if constexpr (order == QUADRATIC){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }
    }
}

#endif    // FEM_SRC_INTEGRATION_EXTRAPOLATE_H_
