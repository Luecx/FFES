
#include "Iso2DTri3.h"

#include "../assert/Error.h"
#include "../integration/Quadrature.h"
#include "../system/LoadCase.h"

Iso2DTri3::Iso2DTri3(int node_1, int node_2, int node_3) {
    node_ids[0] = node_1;
    node_ids[1] = node_2;
    node_ids[2] = node_3;
}

QuickMatrix<3, 2> Iso2DTri3::getLocalShapeDerivative(Precision r, Precision s, Precision t) {
    QuickMatrix<3, 2> res {};
    res(0, 0) = 1;
    res(0, 1) = 0;

    res(1, 0) = 0;
    res(1, 1) = 1;

    res(2, 0) = -1;
    res(2, 1) = -1;

    return res;
}

QuickMatrix<3, 6> Iso2DTri3::computeStrainDisplacementRelationFromSource(QuickMatrix<3, 2> b_help) {
    QuickMatrix<3, 6> B {};
    B(0, 0) = b_help(0,0);
    B(0, 2) = b_help(1,0);
    B(0, 4) = b_help(2,0);
    B(1, 1) = b_help(0,1);
    B(1, 3) = b_help(1,1);
    B(1, 5) = b_help(2,1);
    B(2, 0) = b_help(0,1);
    B(2, 1) = b_help(0,0);
    B(2, 2) = b_help(1,1);
    B(2, 3) = b_help(1,0);
    B(2, 4) = b_help(2,1);
    B(2, 5) = b_help(2,0);
    return B;
}

DenseMatrix       Iso2DTri3::getIntegrationScheme() { return integrate<ISO_TRI, LINEAR>(); }
QuickMatrix<3, 1> Iso2DTri3::getShapeFunction(Precision r, Precision s, Precision t) {
    QuickMatrix<3, 1> res {};
    res(0, 0) = r;
    res(1, 0) = s;
    res(2, 0) = 1 - r - s;
    return res;
}
QuickMatrix<3, 2> Iso2DTri3::getNodeLocalCoordinates() {
    QuickMatrix<3, 2> res {};

    res(0, 0) = 1;
    res(0, 1) = 0;
    res(1, 0) = 0;
    res(1, 1) = 1;
    res(2, 0) = 0;
    res(2, 1) = 0;
    return res;
}
