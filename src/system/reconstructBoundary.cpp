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
// Created by Luecx on 16.09.2021.
//
#include "LoadCase.h"

void LoadCase::reconstructBoundary(){
    if(previous_load_case == nullptr) return;

//  BOUNDARY_IS_CONSTRAINED_NEXT_CASE,
//  BOUNDARY_IS_CONSTRAINED,
//  BOUNDARY_DISPLACEMENT_NEXT_CASE,
//  BOUNDARY_DISPLACEMENT,
//  BOUNDARY_FORCE_NEXT_CASE,
//  BOUNDARY_FORCE,

    node_data[BOUNDARY_IS_CONSTRAINED]
        = node_data[BOUNDARY_IS_CONSTRAINED_NEXT_CASE]
        = previous_load_case->node_data[BOUNDARY_IS_CONSTRAINED_NEXT_CASE];

    node_data[BOUNDARY_DISPLACEMENT]
        = node_data[BOUNDARY_DISPLACEMENT_NEXT_CASE]
        = previous_load_case->node_data[BOUNDARY_DISPLACEMENT_NEXT_CASE];

    node_data[BOUNDARY_FORCE]
        = node_data[BOUNDARY_FORCE_NEXT_CASE]
        = previous_load_case->node_data[BOUNDARY_FORCE_NEXT_CASE];

}
