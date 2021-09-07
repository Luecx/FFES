//
// Created by Luecx on 05.09.2021.
//
#include "Iso2DTri3.h"

#include "../assert/Error.h"
Iso2DTri3::Iso2DTri3(int node_1, int node_2, int node_3)
    : node1(node_1), node2(node_2), node3(node_3) {}

DenseMatrix Iso2DTri3::computeLocalStiffness() {

    //        this->updateJacobians();
    /**
     * to fill B, we need to compute:
     *
     *
     *  d/dx  *  (h1 h2 h3) = J^-1 * d/dr * (h1 h2 h3)
     *  d/dy                         d/ds
     *
     *
     *                      = J^-1 *  -1  1  0
     *                                -1  0  1
     *
     *                      = J^-1 *   G
     */

    QuickMatrix<2, 3> G {};
    G(0, 0) = -1;
    G(0, 1) = 1;
    G(0, 2) = 0;
    G(1, 0) = -1;
    G(1, 1) = 0;
    G(1, 2) = 1;

    Precision            det;
    QuickMatrix<2, 2> jacobian {computeJacobian()};
    QuickMatrix<2, 2> inverseJacobian = inverse(jacobian, det);

    ERROR(det > 0, NEGATIVE_JACOBIAN);
    ERROR(material, NO_MATERIAL_ASSIGNED);

    auto              BHelp           = inverseJacobian * G;

    QuickMatrix<3, 6> B {};
    B(0, 0) = BHelp(0, 0);
    B(0, 2) = BHelp(0, 1);
    B(0, 4) = BHelp(0, 2);
    B(1, 1) = BHelp(1, 0);
    B(1, 3) = BHelp(1, 1);
    B(1, 5) = BHelp(1, 2);
    B(2, 0) = BHelp(1, 0);
    B(2, 1) = BHelp(0, 0);
    B(2, 2) = BHelp(1, 1);
    B(2, 3) = BHelp(0, 1);
    B(2, 4) = BHelp(1, 2);
    B(2, 5) = BHelp(0, 2);

    QuickMatrix<6, 6> stiffness = (!B) * material->getMaterialMatrix2D() * B;
    // 0.5 due to integration over isoparametric triangle is 0.5
    stiffness *= 1 * 0.5 * det;

    return DenseMatrix {stiffness};
}

QuickMatrix<2, 2> Iso2DTri3::computeJacobian() {
    QuickMatrix<2, 2> jacobian {};

    Precision            x1 = (*node_data)[POSITION][node1][0];
    Precision            y1 = (*node_data)[POSITION][node1][1];

    Precision            x2 = (*node_data)[POSITION][node2][0];
    Precision            y2 = (*node_data)[POSITION][node2][1];

    Precision            x3 = (*node_data)[POSITION][node3][0];
    Precision            y3 = (*node_data)[POSITION][node3][1];

    /**    --                    --
     *     | -x1 + x2    -y1 + y2 |
     * J = |                      |
     *     | -x1 + x3    -y1 + y3 |
     *     --                    --
     */

    jacobian(0, 0)       = -x1 + x2;
    jacobian(1, 0)       = -x1 + x3;

    jacobian(0, 1)       = -y1 + y2;
    jacobian(1, 1)       = -y1 + y3;

    return jacobian;
}
