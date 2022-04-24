
//
// Created by Luecx on 01.10.2021.
//
#include "Iso2DQuad8.h"

#include "../integration/Quadrature.h"
Iso2DQuad8::Iso2DQuad8(int node_1,
                       int node_2,
                       int node_3,
                       int node_4,
                       int node_5,
                       int node_6,
                       int node_7,
                       int node_8) {
    node_ids[0] = node_1;
    node_ids[1] = node_2;
    node_ids[2] = node_3;
    node_ids[3] = node_4;
    node_ids[4] = node_5;
    node_ids[5] = node_6;
    node_ids[6] = node_7;
    node_ids[7] = node_8;
}


QuickMatrix<8, 2> Iso2DQuad8::getLocalShapeDerivative(Precision r, Precision s, Precision t) {
    Precision rm = 1 - r;
    Precision sm = 1 - s;
    Precision rp = 1 + r;
    Precision sp = 1 + s;

    QuickMatrix<8,2> result{};
    result(0,0) = 1.0/4.0 * sm * (-1 * (-r-s-1) + -1 * rm);
    result(1,0) = 1.0/4.0 * sm * ( 1 * ( r-s-1) +  1 * rp);
    result(2,0) = 1.0/4.0 * sp * ( 1 * ( r+s-1) +  1 * rp);
    result(3,0) = 1.0/4.0 * sp * (-1 * (-r+s-1) + -1 * rm);

    result(0,1) = 1.0/4.0 * rm * (-1 * (-r-s-1) + -1 * sm);
    result(1,1) = 1.0/4.0 * rp * (-1 * ( r-s-1) + -1 * sm);
    result(2,1) = 1.0/4.0 * rp * ( 1 * ( r+s-1) +  1 * sp);
    result(3,1) = 1.0/4.0 * rm * ( 1 * (-r+s-1) +  1 * sp);

    result(4,0) =  1.0/2.0 * sm * ( 1 * rm + -1 * rp);
    result(5,0) =  1.0/2.0 * sm * sp;
    result(6,0) =  1.0/2.0 * sp * ( 1 * rm + -1 * rp);
    result(7,0) = -1.0/2.0 * sm * sp;

    result(4,1) = -1.0/2.0 * rm * rp;
    result(5,1) =  1.0/2.0 * rp * ( 1 * sm + -1 * sp);
    result(6,1) =  1.0/2.0 * rm * rp;
    result(7,1) =  1.0/2.0 * rm * ( 1 * sm + -1 * sp);

    return result;
}
QuickMatrix<3, 16> Iso2DQuad8::computeStrainDisplacementRelationFromSource(QuickMatrix<8, 2> b_help) {
    QuickMatrix<3,16> B{};
    for(int j = 0; j < 8; j++){
        int r1 = j * 2;
        int r2 = r1 + 1;
        B(0,r1) = b_help(j,0);
        B(1,r2) = b_help(j,1);
        B(2,r2) = b_help(j,0);
        B(2,r1) = b_help(j,1);
    }
    return B;
}
DenseMatrix Iso2DQuad8::getIntegrationScheme() { return integrate<ISO_QUAD, QUADRATIC>(); }
QuickMatrix<8, 1> Iso2DQuad8::getShapeFunction(Precision r, Precision s, Precision t) {
    QuickMatrix<8, 1> res {};
    res(0,0) = 0.25*(1-r)*(1-s)*(-r-s-1);
    res(1,0) = 0.25*(1+r)*(1-s)*( r-s-1);
    res(2,0) = 0.25*(1+r)*(1+s)*( r+s-1);
    res(3,0) = 0.25*(1-r)*(1+s)*(-r+s-1);

    res(4,0) = 0.5*(1-s)*(1+r)*(1-r);
    res(5,0) = 0.5*(1+r)*(1+s)*(1-s);
    res(6,0) = 0.5*(1+s)*(1+r)*(1-r);
    res(7,0) = 0.5*(1-r)*(1+s)*(1-s);

    return res;
}
QuickMatrix<8, 2> Iso2DQuad8::getNodeLocalCoordinates() {
    QuickMatrix<8, 2> res{};
    res(0,0) = -1;
    res(0,1) = -1;
    res(1,0) =  1;
    res(1,1) = -1;
    res(2,0) =  1;
    res(2,1) =  1;
    res(3,0) = -1;
    res(3,1) =  1;

    res(4,0) =  0;
    res(4,1) = -1;
    res(5,0) =  1;
    res(5,1) =  0;
    res(6,0) =  0;
    res(6,1) =  1;
    res(7,0) = -1;
    res(7,1) =  0;
    return res;
}
