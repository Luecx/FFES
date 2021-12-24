
#include "Iso2DTri6.h"

#include "../assert/Error.h"
#include "../integration/Quadrature.h"
#include "../system/LoadCase.h"

Iso2DTri6::Iso2DTri6(int node_1, int node_2, int node_3,int node_4, int node_5, int node_6){
    node_ids[0] = node_1;
    node_ids[1] = node_2;
    node_ids[2] = node_3;
    node_ids[1] = node_4;
    node_ids[2] = node_5;
    node_ids[1] = node_6;
}

//QuickMatrix<2, 3> Iso2DTri6::getLocalShapeDerivative(Precision r, Precision s, Precision t) {
//    QuickMatrix<2, 3> res{};
//    res(0,0) = 1;
//    res(1,0) = 0;
//
//    res(0,1) = 0;
//    res(1,1) = 1;
//
//    res(0,2) = -1;
//    res(1,2) = -1;
//
//
//    return res;
//}
//
//QuickMatrix<3, 6> Iso2DTri6::computeStrainDisplacementRelationFromSource(QuickMatrix<2, 3> b_help) {
//    QuickMatrix<3, 6> B {};
//    B(0, 0) = b_help(0, 0);
//    B(0, 2) = b_help(0, 1);
//    B(0, 4) = b_help(0, 2);
//    B(1, 1) = b_help(1, 0);
//    B(1, 3) = b_help(1, 1);
//    B(1, 5) = b_help(1, 2);
//    B(2, 0) = b_help(1, 0);
//    B(2, 1) = b_help(0, 0);
//    B(2, 2) = b_help(1, 1);
//    B(2, 3) = b_help(0, 1);
//    B(2, 4) = b_help(1, 2);
//    B(2, 5) = b_help(0, 2);
//    return B;
//}
//
//DenseMatrix Iso2DTri6::getIntegrationScheme() { return integrate<ISO_TRI, LINEAR>(); }
//QuickMatrix<3, 1> Iso2DTri6::getShapeFunction(Precision r, Precision s, Precision t) {
//    QuickMatrix<3,1> res{};
//    res(0,0) = r;
//    res(1,0) = s;
//    res(2,0) = 1-r-s;
//    return res;
//}
//QuickMatrix<3, 2> Iso2DTri6::getNodeLocalCoordinates() {
//    QuickMatrix<3, 2> res{};
//
//    res(0,0) = 1;
//    res(0,1) = 0;
//    res(1,0) = 0;
//    res(1,1) = 1;
//    res(2,0) = 0;
//    res(2,1) = 0;
//    return res;
//}
QuickMatrix<6, 1> Iso2DTri6::getShapeFunction(Precision r, Precision s, Precision t) {
    QuickMatrix<6, 1> res{};


    return res;
}
QuickMatrix<6 ,2> Iso2DTri6::getLocalShapeDerivative(Precision r, Precision s, Precision t) {
    QuickMatrix<6,2 > res{};

    return res;
}
QuickMatrix<3, 12> Iso2DTri6::computeStrainDisplacementRelationFromSource(QuickMatrix<6, 2> b_help) {
    return QuickMatrix<3, 12>();
}
DenseMatrix       Iso2DTri6::getIntegrationScheme() { return DenseMatrix(0, 0); }
QuickMatrix<6, 2> Iso2DTri6::getNodeLocalCoordinates() { return QuickMatrix<6, 2>(); }
