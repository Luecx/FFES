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

#ifndef FEM_SRC_ENTITY_ISO2DQUAD4_H_
#define FEM_SRC_ENTITY_ISO2DQUAD4_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"

struct Iso2DQuad4 : public Element {

    int node_ids[4] {};
    Iso2DQuad4(int node_1, int node_2, int node_3, int node_4);

    DenseMatrix computeLocalStiffness() override;
    DenseMatrix computeStress(LoadCase* load_case, const DenseMatrix& evaluation_points) override;
    DenseMatrix getIntegrationScheme() override;

    int         nodeDOF() override;
    int         nodeCount() override;
    int*        nodeIDS() override;
};

#endif    // FEM_SRC_ENTITY_ISO2DQUAD4_H_
