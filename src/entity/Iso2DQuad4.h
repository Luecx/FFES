//
// Created by Luecx on 03.09.2021.
//

#ifndef FEM_SRC_ENTITY_ISO2DQUAD4_H_
#define FEM_SRC_ENTITY_ISO2DQUAD4_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"

struct Iso2DQuad4 : public Element{

    int node1, node2, node3, node4;

    Iso2DQuad4(int node_1, int node_2, int node_3, int node_4);

    DenseMatrix computeLocalStiffness() override;
};

#endif    // FEM_SRC_ENTITY_ISO2DQUAD4_H_
