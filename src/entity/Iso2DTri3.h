//
// Created by Luecx on 03.09.2021.
//

#ifndef FEM_SRC_ENTITY_ISO2DTRI3_H_
#define FEM_SRC_ENTITY_ISO2DTRI3_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"

struct Iso2DTri3 : public Element{

    int node1, node2, node3;

    Iso2DTri3(int node_1, int node_2, int node_3);

    DenseMatrix computeLocalStiffness() override;
    QuickMatrix<2,2> computeJacobian();
};

#endif    // FEM_SRC_ENTITY_ISO2DTRI3_H_
