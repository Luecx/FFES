/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFES is free software: you can redistribute it and/or modify                  *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFES is distributed in the hope that it will be useful,                    *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

//
// Created by Luecx on 27.09.2021.
//

#ifndef FEM_SRC_ENTITY_ISOELEMENT_H_
#define FEM_SRC_ENTITY_ISOELEMENT_H_

#include "../system/LoadCase.h"
#include "Element.h"

#define DIM_TRAF(D) (D == 2 ? 3 : 6)

template<int N, int D>
struct IsoElement : public Element {

    ID node_ids[N] {};

    // computes the shape functions at position r,s,t for each node
    virtual QuickMatrix<N, 1> getShapeFunction(Precision r, Precision s, Precision t)            = 0;

    // containing derivatives of the shape functions h1 -> h8 with respect to r/s/t
    virtual QuickMatrix<N, D> getLocalShapeDerivative(Precision r, Precision s, Precision t = 0) = 0;

    // computes the strain displacement matrix B from source
    // (already knowing the entries but just shaping)
    virtual QuickMatrix<DIM_TRAF(D), N * D>
                        computeStrainDisplacementRelationFromSource(QuickMatrix<N, D> b_help) = 0;

    // returns the integration scheme over this element
    virtual DenseMatrix getIntegrationScheme()                                                = 0;

    // returns the local coordinates of the nodes
    virtual QuickMatrix<N,D> getNodeLocalCoordinates()                                        = 0;

    // containing derivatives of x,y,z with respect to r/s/t
    virtual QuickMatrix<D, D> getJacobian(QuickMatrix<N, D>& node_coords,
                                          Precision r,
                                          Precision s,
                                          Precision t = 0);

    // containing derivatives of x,y,z with respect to r/s/t
    virtual QuickMatrix<D, D> getJacobian(Precision r, Precision s, Precision t);

    // computes the relation between the strain and displacement given the node_coords
    virtual QuickMatrix<DIM_TRAF(D), N * D>
        computeStrainDisplacementRelation(QuickMatrix<N, D>& node_coords,
                                          Precision&         det,
                                          Precision          r,
                                          Precision          s,
                                          Precision          t = 0);

    // returns the material matrix and adjusts it by simp factors
    virtual QuickMatrix<DIM_TRAF(D), DIM_TRAF(D)> getAdjustedMaterialMatrix(LoadCase* load_case = nullptr);

    Precision   compliance(LoadCase* load_case) override;
    Precision   interpolate(DenseMatrix nodal, Precision r, Precision s, Precision t) override;
    DenseMatrix computeLocalStiffness(LoadCase* load_case) override;
    DenseMatrix computeLocalMassMatrix(LoadCase* load_case) override;
    DenseMatrix computeStressAtNodes(LoadCase* load_case) override;

    int         nodeDOF() override;
    int         nodeCount() override;
    int*        nodeIDS() override;

    DenseMatrix getNodalData(NodeDataEntries entry, NodeData* p_node_data, int dim_count) override;
    DenseMatrix getNodalData(NodeDataEntries entry, LoadCase* load_case, int dim_count) override;
    DenseMatrix getNodalData(NodeDataEntries entry, int dim_count) override;

    template<int DIMS = D>
    QuickMatrix<N * DIMS, 1> getNodalData(NodeDataEntries entry, NodeData* p_node_data);
    template<int DIMS = D>
    QuickMatrix<N * DIMS, 1> getNodalData(NodeDataEntries entry, LoadCase* load_case);
    template<int DIMS = D>
    QuickMatrix<N * DIMS, 1> getNodalData(NodeDataEntries entry);
};

#endif    // FEM_SRC_ENTITY_ISOELEMENT_H_
