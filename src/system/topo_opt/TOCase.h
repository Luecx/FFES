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

#ifndef FEM_SRC_SYSTEM_TOPO_OPT_TOCASE_H_
#define FEM_SRC_SYSTEM_TOPO_OPT_TOCASE_H_

#include "../LoadCase.h"

struct TOCase : public LoadCase{

    TOCase(Model* p_model, LoadCase* p_previous, int iterations, Precision min_res) : LoadCase(p_model, p_previous) {
        this->element_data[SIMP_DENSITY_FACTOR]
            .init(model->max_element_count, model->max_element_count)
            .even()
            .fill(1);
        this->element_data[SIMP_DENSITY_EXPONENT]
            .init(model->max_element_count, model->max_element_count)
            .even()
            .fill(3);
    }

    virtual ~TOCase() {
        this->element_data[SIMP_DENSITY_EXPONENT].cleanUp();
        this->element_data[SIMP_DENSITY_FACTOR  ].cleanUp();
    }
};

#endif    // FEM_SRC_SYSTEM_TOPO_OPT_TOCASE_H_
