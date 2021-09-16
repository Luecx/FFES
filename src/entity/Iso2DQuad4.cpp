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
#include "Iso2DQuad4.h"

#include "../assert/Error.h"
#include "../integration/Quadrature.h"
Iso2DQuad4::Iso2DQuad4(int node_1, int node_2, int node_3, int node_4)
    : node_ids {node_1, node_2, node_3, node_4} {}

DenseMatrix Iso2DQuad4::computeLocalStiffness() {

    QuickMatrix<8, 8> stiffness {};

    ERROR(material, MATERIAL_HAS_NO_ELASTICITY, 0);
    ERROR(material->getElasticity(), NO_MATERIAL_ASSIGNED, 0);

    DenseMatrix       integration_points = integrate<ISO_QUAD, LINEAR>();
    auto              mat_matrix         = material->getElasticity()->getMaterialMatrix2D();

    auto node1 = node_ids[0];
    auto node2 = node_ids[1];
    auto node3 = node_ids[2];
    auto node4 = node_ids[3];

    Precision         x1                 = (*node_data)[POSITION][node1][0];
    Precision         y1                 = (*node_data)[POSITION][node1][1];

    Precision         x2                 = (*node_data)[POSITION][node2][0];
    Precision         y2                 = (*node_data)[POSITION][node2][1];

    Precision         x3                 = (*node_data)[POSITION][node3][0];
    Precision         y3                 = (*node_data)[POSITION][node3][1];

    Precision         x4                 = (*node_data)[POSITION][node4][0];
    Precision         y4                 = (*node_data)[POSITION][node4][1];

    for (int i = 0; i < integration_points.getM(); i++) {
        Precision        r = integration_points(i, 0);
        Precision s = integration_points(i,1);

        // containing derivatives of the shape functions h1 -> h4 with respect to r/s
        QuickMatrix<2,4> local_shape_derivative{};
        local_shape_derivative(0,0) =  s + 1;
        local_shape_derivative(1,0) =  r + 1;
        local_shape_derivative(0,1) = -s - 1;
        local_shape_derivative(1,1) =  1 - r;
        local_shape_derivative(0,2) =  s - 1;
        local_shape_derivative(1,2) =  r - 1;
        local_shape_derivative(0,3) =  1 - s;
        local_shape_derivative(1,3) = -r - 1;
        local_shape_derivative *= 0.25;

        // compute jacobian
        QuickMatrix<2,2> jacobian{};
        jacobian(0,0) =
            + local_shape_derivative(0,0) * x1
            + local_shape_derivative(0,1) * x2
            + local_shape_derivative(0,2) * x3
            + local_shape_derivative(0,3) * x4;
        jacobian(0,1) =
            + local_shape_derivative(0,0) * y1
            + local_shape_derivative(0,1) * y2
            + local_shape_derivative(0,2) * y3
            + local_shape_derivative(0,3) * y4;
        jacobian(1,1) =
            + local_shape_derivative(1,0) * y1
            + local_shape_derivative(1,1) * y2
            + local_shape_derivative(1,2) * y3
            + local_shape_derivative(1,3) * y4;
        jacobian(1,0) =
            + local_shape_derivative(1,0) * x1
            + local_shape_derivative(1,1) * x2
            + local_shape_derivative(1,2) * x3
            + local_shape_derivative(1,3) * x4;

        // compute inverse jacobian + determinant
        Precision det;
        auto inv = inverse2x2(jacobian, det);

        // error handling
        ERROR(det > 0, NEGATIVE_JACOBIAN, det);

        // compute derivatives with respect to x,y
        auto B_help = inv * local_shape_derivative;

        // formulating Strain-displacement matrix
        QuickMatrix<3,8> B{};
        for(int j = 0; j < 4; j++){
            int r1 = j * 2;
            int r2 = r1 + 1;
            B(0,r1) = B_help(0,j);
            B(1,r2) = B_help(1,j);
            B(2,r2) = B_help(0,j);
            B(2,r1) = B_help(1,j);
        }

        stiffness += (!B * mat_matrix * B) * (integration_points(i, 2) * det * 1);
    }

    return DenseMatrix{stiffness};

}

DenseMatrix Iso2DQuad4::extrapolate(DenseMatrix& integration_point_results) {
    //TODO
    return integration_point_results;
}

int Iso2DQuad4::nodeCount() {return 4;}
int* Iso2DQuad4::nodeIDS() { return node_ids; }
int  Iso2DQuad4::nodeDOF() { return 2; }

