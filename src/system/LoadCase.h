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

#ifndef FEM_SRC_SYSTEM_LOADCASE_H_
#define FEM_SRC_SYSTEM_LOADCASE_H_

#include "../assert/Assert.h"
#include "../assert/Error.h"
#include "../core/ElementData.h"
#include "../core/NodeData.h"
#include "../model/Model.h"
#include "../solver/CG.h"

struct LoadCase {
    NodeData    node_data {};
    ElementData element_data {};
    Model*      model;
    LoadCase*   previous_load_case;

    LoadCase(Model* p_model, LoadCase* p_previous = nullptr)
        : model(p_model), previous_load_case(p_previous) {
        // reconstruct boundaries
        reconstructBoundary();
    }

    // functions to constraint nodes
    void constraint(const std::string& set, Precision x = NAN, Precision y = NAN, Precision z = NAN,bool isTemp = true);
    void constraint(const std::string& set, int dimension, Precision value = NAN, bool isTemp = true);
    void constraint(int node_id, Precision x = NAN, Precision y = NAN, Precision z = NAN,bool isTemp = true);
    void constraint(int node_id, int dimension, Precision value = NAN, bool isTemp = true);

    // functions to apply loads to nodes
    void applyLoad(const std::string& set, Precision x = NAN, Precision y = NAN, Precision z = NAN,bool isTemp = true);
    void applyLoad(const std::string& set, int dimension, Precision value = NAN, bool isTemp = true);
    void applyLoad(int node_id, Precision x = NAN, Precision y = NAN, Precision z = NAN,bool isTemp = true);
    void applyLoad(int node_id, int dimension, Precision value = NAN, bool isTemp = true);

    // functions to apply simp factor to elements
    void simp(const std::string& set, Precision rho = 1, Precision p = 1,bool isTemp = true);
    void simp(int element_id, Precision rho = 1, Precision p = 1,bool isTemp = true);


    void reconstructBoundary();

    virtual void compute(){
        model->numerateUnconstrainedNodes(this);
        auto matrix = model->buildReducedStiffnessMatrix(this);
        auto vector = model->buildReducedLoadVector(this);
        auto solution = conjugate_gradient(matrix, vector);
        model->postProcessDisplacements(solution, this);
        model->computeStressAtNodes(this);
    }
};

#endif    // FEM_SRC_SYSTEM_LOADCASE_H_
