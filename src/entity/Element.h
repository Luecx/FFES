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

#ifndef FEM_SRC_ENTITY_ELEMENT_H_
#define FEM_SRC_ENTITY_ELEMENT_H_

#include "../core/NodeData.h"
#include "../material/Material.h"
#include "../matrix/DenseMatrix.h"

struct LoadCase;

struct Element {

    NodeData* node_data = nullptr;
    Material* material = nullptr;

    virtual ~Element() {}

    virtual DenseMatrix computeLocalStiffness() = 0;
    virtual DenseMatrix computeStress(LoadCase* load_case, const DenseMatrix& evaluation_points) = 0;
    virtual DenseMatrix getIntegrationScheme() = 0;

    virtual int  nodeDOF() = 0;
    virtual int  nodeCount() = 0;
    virtual int* nodeIDS() = 0;
};


#endif    // FEM_SRC_ENTITY_ELEMENT_H_
