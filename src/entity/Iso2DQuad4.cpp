
#include "Iso2DQuad4.h"

#include "../assert/Error.h"
#include "../integration/Quadrature.h"
#include "../system/LoadCase.h"

Iso2DQuad4::Iso2DQuad4(int node_1, int node_2, int node_3, int node_4) {
    node_ids[0] = node_1;
    node_ids[1] = node_2;
    node_ids[2] = node_3;
    node_ids[3] = node_4;
}
QuickMatrix<4, 2> Iso2DQuad4::getLocalShapeDerivative(Precision r, Precision s, Precision t) {
    QuickMatrix<4, 2> local_shape_derivative {};
    local_shape_derivative(0, 0) = s - 1;
    local_shape_derivative(0, 1) = r - 1;
    local_shape_derivative(1, 0) = 1 - s;
    local_shape_derivative(1, 1) = -r - 1;
    local_shape_derivative(2, 0) = 1 + s;
    local_shape_derivative(2, 1) = 1 + r;
    local_shape_derivative(3, 0) = -s - 1;
    local_shape_derivative(3, 1) = 1 - r;
    local_shape_derivative *= 0.25;
    return local_shape_derivative;
}
QuickMatrix<3, 8> Iso2DQuad4::computeStrainDisplacementRelationFromSource(QuickMatrix<4, 2> b_help) {
    QuickMatrix<3, 8> B {};
    for (int j = 0; j < 4; j++) {
        int r1   = j * 2;
        int r2   = r1 + 1;
        B(0, r1) = b_help(j, 0);
        B(1, r2) = b_help(j, 1);
        B(2, r2) = b_help(j, 0);
        B(2, r1) = b_help(j, 1);
    }
    return B;
}
DenseMatrix       Iso2DQuad4::getIntegrationScheme() { return integrate<ISO_QUAD, LINEAR>(); }
QuickMatrix<4, 1> Iso2DQuad4::getShapeFunction(Precision r, Precision s, Precision t) {
    QuickMatrix<4, 1> res {};

    res(0, 0) = (1 - r) * (1 - s);
    res(1, 0) = (1 + r) * (1 - s);
    res(2, 0) = (1 + r) * (1 + s);
    res(3, 0) = (1 - r) * (1 + s);

    res *= 0.25;
    return res;
}
QuickMatrix<4, 2> Iso2DQuad4::getNodeLocalCoordinates() {
    QuickMatrix<4, 2> res {};
    res(0, 0) = -1;
    res(0, 1) = -1;
    res(1, 0) = 1;
    res(1, 1) = -1;
    res(2, 0) = 1;
    res(2, 1) = 1;
    res(3, 0) = -1;
    res(3, 1) = 1;
    return res;
}
