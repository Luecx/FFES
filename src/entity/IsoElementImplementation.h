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

#ifndef FEM_SRC_ENTITY_ISOELEMENTIMPLEMENTATION_H_
#define FEM_SRC_ENTITY_ISOELEMENTIMPLEMENTATION_H_

#include "IsoElement.h"
#include "../model/Model.h"

template<int N, int D>
QuickMatrix<D, D> IsoElement<N, D>::getJacobian(QuickMatrix<N, D>& node_coords,
                                                Precision          r,
                                                Precision          s,
                                                Precision          t) {

    QuickMatrix<D, N> local_shape_derivative = getLocalShapeDerivative(r, s, t);
    QuickMatrix<D, D> jacobian {};
    for (int j = 0; j < D; j++) {
        for (int n = 0; n < D; n++) {
            Precision sum = 0;
            for (int b = 0; b < N; b++) {
                sum += node_coords(b, n) * local_shape_derivative(j, b);
            }
            jacobian(j, n) = sum;
        }
    }
    return jacobian;
}

template<int N, int D>
QuickMatrix<D, D> IsoElement<N, D>::getJacobian(Precision r,
                                                Precision s,
                                                Precision t) {

    auto nd_data  = getNodalData<D>(POSITION);
    auto reshaped = nd_data.template reshape<N, D>();
    return getJacobian(reshaped, r, s, t);
}

template<int N, int D>
QuickMatrix<DIM_TRAF(D), N * D> IsoElement<N, D>::computeStrainDisplacementRelation(
                                                QuickMatrix<N, D>& node_coords,
                                                Precision&         det,
                                                Precision          r,
                                                Precision          s,
                                                Precision          t) {

    auto              local_shape_derivative = getLocalShapeDerivative(r, s, t);
    auto              jacobian               = getJacobian(node_coords, r, s, t);

    QuickMatrix<D, D> inv;
    if constexpr (D == 3) {
        inv = inverse3x3(jacobian, det);
    } else if constexpr (D == 2) {
        inv = inverse2x2(jacobian, det);
    } else {
        inv = inverse(jacobian, det);
    }

    // error handling
    ERROR(det > 0, NEGATIVE_JACOBIAN, det);

    auto B_help = inv * local_shape_derivative;
    return computeStrainDisplacementRelationFromSource(B_help);
}

template<int N, int D>
QuickMatrix<DIM_TRAF(D), DIM_TRAF(D)> IsoElement<N, D>::getAdjustedMaterialMatrix(
                                                LoadCase* load_case) {

    ERROR(material, NO_MATERIAL_ASSIGNED, "No Material assigned");
    ERROR(material->getElasticity(), MATERIAL_HAS_NO_ELASTICITY, "No Elasticity for material");

    constexpr auto        DIM = D == 2 ? 3 : 6;

    QuickMatrix<DIM, DIM> mat_matrix;
    if constexpr (D == 2) {
        mat_matrix = material->getElasticity()->getMaterialMatrix2D();
    } else if constexpr (D == 3) {
        mat_matrix = material->getElasticity()->getMaterialMatrix3D();
    } else {
        ERROR(material->getElasticity(), NO_MATERIAL_ASSIGNED, 0);
    }

    // adjust material based on simp scaling. Scaled by rho^p where rho is a density scalar
    // between 0 and 1. p is a penality factor.
    if (load_case != nullptr && load_case->element_data[SIMP_DENSITY_FACTOR].isInitialised()
        && load_case->element_data[SIMP_DENSITY_EXPONENT].isInitialised()) {
        mat_matrix *= pow(load_case->element_data[SIMP_DENSITY_FACTOR][element_id][0],
                          load_case->element_data[SIMP_DENSITY_EXPONENT][element_id][0]);
    }

    return mat_matrix;
}

template<int N, int D>
DenseMatrix IsoElement<N, D>::computeLocalStiffness(LoadCase* load_case) {
    QuickMatrix<N * D, N * D> stiffness {};
    auto                      mat         = getAdjustedMaterialMatrix(load_case);
    auto                      node_coords = getNodalData(POSITION).template reshape<N, D>();
    auto                      integration = getIntegrationScheme();

    for (int i = 0; i < integration.getM(); i++) {
        Precision r = integration(i, 0);
        Precision s = integration(i, 1);
        Precision t = integration(i, 2);

        if (D == 2)
            t = 0;

        Precision det = 0;
        auto      B   = computeStrainDisplacementRelation(node_coords, det, r, s, t);
        stiffness += (!B * mat * B) * (integration(i, integration.getN() - 1) * det
                                       * model->element_data[ELEMENT_THICKNESS][element_id][0]);
    }

    return stiffness;
}

template<int N, int D>
DenseMatrix IsoElement<N, D>::computeLocalMassMatrix(LoadCase* load_case) {
//    QuickMatrix<N * D, N * D> mass_matrix {};
//    auto                      mat         = getAdjustedMaterialMatrix(load_case);
//    auto                      node_coords = getNodalData(POSITION).template reshape<N, D>();
//    auto                      integration = getIntegrationScheme();
//
//    for (int i = 0; i < integration.getM(); i++) {
//        Precision r = integration(i, 0);
//        Precision s = integration(i, 1);
//        Precision t = integration(i, 2);
//
//        if (D == 2)
//            t = 0;
//
//        Precision det = 0;
//        auto      B   = computeStrainDisplacementRelation(node_coords, det, r, s, t);
//        stiffness += (!B * mat * B) * (integration(i, integration.getN() - 1) * det
//            * model->element_data[ELEMENT_THICKNESS][element_id][0]);
//    }
//
//    return stiffness;
    // TODO
}

template<int N, int D>
DenseMatrix IsoElement<N, D>::computeStressAtNodes(LoadCase* load_case) {

    DenseMatrix stresses {N, DIM_TRAF(D)};
    auto        mat_matrix  = getAdjustedMaterialMatrix(load_case);

    auto        node_coords = getNodalData(POSITION).template reshape<N, D>();
    auto        disp_vector = getNodalData(DISPLACEMENT, load_case);

    auto        points      = this->getNodeLocalCoordinates();

    for (int i = 0; i < points.getM(); i++) {
        Precision r      = points.get(i, 0);
        Precision s      = points.get(i, 1);
        Precision t      = points.get(i, 2);
        Precision det    = 0;

        if (D == 2)
            t = 0;

        auto      B      = computeStrainDisplacementRelation(node_coords, det, r, s, t);
        auto      strain = B * disp_vector;
        auto      stress = mat_matrix * strain;

        for (int j = 0; j < stress.getM(); j++) {
            stresses(i, j) = stress(j, 0);
        }
    }
    return stresses;
}

template<int N, int D>
int IsoElement<N, D>::nodeDOF() { return D; }

template<int N, int D>
int IsoElement<N, D>::nodeCount() { return N; }

template<int N, int D>
int* IsoElement<N, D>::nodeIDS() { return node_ids; }

template<int N, int D>
Precision IsoElement<N, D>::compliance(LoadCase* load_case) {
    auto stiffness = computeLocalStiffness(load_case);
    auto disp      = DenseMatrix(getNodalData(DISPLACEMENT, load_case));
    return (!disp * stiffness * disp)(0,0);
}

template<int N, int D>
Precision IsoElement<N, D>::interpolate(DenseMatrix nodal, Precision r, Precision s, Precision t) {
    auto shape_function = this->getShapeFunction(r,s,t);
    Precision res{};
    for(int i = 0; i < shape_function.getM(); i++){
        res += shape_function(i,0) * nodal(i,0);
    }
    return 0;
}

template<int N, int D>
DenseMatrix IsoElement<N, D>::getNodalData(NodeDataEntries entry,
                                           NodeData* p_node_data,
                                           int dim_count) {
    switch (dim_count) {
        case 6:
            return getNodalData<6>(entry, p_node_data);
        case 5:
            return getNodalData<5>(entry, p_node_data);
        case 4:
            return getNodalData<4>(entry, p_node_data);
        case 3:
            return getNodalData<3>(entry, p_node_data);
        case 2:
            return getNodalData<2>(entry, p_node_data);
        case 1:
            return getNodalData<1>(entry, p_node_data);
        default:
            return getNodalData<1>(entry, p_node_data);
    }
}
template<int N, int D>
DenseMatrix IsoElement<N, D>::getNodalData(NodeDataEntries entry,
                                           LoadCase* load_case,
                                           int dim_count) {
    switch (dim_count) {
        case 6:
            return getNodalData<6>(entry, load_case);
        case 5:
            return getNodalData<5>(entry, load_case);
        case 4:
            return getNodalData<4>(entry, load_case);
        case 3:
            return getNodalData<3>(entry, load_case);
        case 2:
            return getNodalData<2>(entry, load_case);
        case 1:
            return getNodalData<1>(entry, load_case);
        default:
            return getNodalData<1>(entry, load_case);
    }
}
template<int N, int D>
DenseMatrix IsoElement<N, D>::getNodalData(NodeDataEntries entry,
                                           int dim_count) {
    switch (dim_count) {
        case 6:
            return getNodalData<6>(entry);
        case 5:
            return getNodalData<5>(entry);
        case 4:
            return getNodalData<4>(entry);
        case 3:
            return getNodalData<3>(entry);
        case 2:
            return getNodalData<2>(entry);
        case 1:
            return getNodalData<1>(entry);
        default:
            return getNodalData<1>(entry);
    }
}

template<int N, int D>
template<int DIMS>
QuickMatrix<N * DIMS, 1> IsoElement<N, D>::getNodalData(NodeDataEntries entry,
                                                        NodeData*       p_node_data) {
    QuickMatrix<N * DIMS, 1> matrix {};
    for (int i = 0; i < nodeCount(); i++) {
        for (int j = 0; j < DIMS; j++) {
            matrix(i * DIMS + j, 0) = (*p_node_data)[entry][nodeIDS()[i]][j];
        }
    }
    return matrix;
}

template<int N, int D>
template<int DIMS>
QuickMatrix<N * DIMS, 1> IsoElement<N, D>::getNodalData(NodeDataEntries entry, LoadCase* load_case) {
    return this->getNodalData<DIMS>(entry, &load_case->node_data);
}

template<int N, int D>
template<int DIMS>
QuickMatrix<N * DIMS, 1> IsoElement<N, D>::getNodalData(NodeDataEntries entry) {
    return this->getNodalData<DIMS>(entry, &this->model->node_data);
}

#endif    // FEM_SRC_ENTITY_ISOELEMENTIMPLEMENTATION_H_
