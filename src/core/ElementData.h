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

#ifndef FEM_SRC_CORE_ELEMENTDATA_H_
#define FEM_SRC_CORE_ELEMENTDATA_H_

#include "ComponentContainer.h"
#include "defs.h"

#include <ostream>

enum ElementDataEntries{
    SIMP_DENSITY_FACTOR,
    SIMP_DENSITY_EXPONENT,
    N_MAX_ELEMENT_FLOAT_ENTRIES
};

enum ElementDataIntegerEntries{
    N_MAX_ELEMENT_INT_ENTRIES
};

struct ElementData{
    ComponentContainer<Precision> float_data[N_MAX_ELEMENT_FLOAT_ENTRIES]{};
    ComponentContainer<int>       int_data  [N_MAX_ELEMENT_INT_ENTRIES]{};

    ComponentContainer<Precision>& operator[](ElementDataEntries entry){
        return float_data[entry];
    }

    ComponentContainer<int>&       operator[](ElementDataIntegerEntries entry){
        return int_data[entry];
    }

};

#endif    // FEM_SRC_CORE_NODEDATA_H_
