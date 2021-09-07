//
// Created by Luecx on 03.09.2021.
//

#ifndef FEM_SRC_ENTITY_Iso3DHex8_H_
#define FEM_SRC_ENTITY_Iso3DHex8_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"

struct Iso3DHex8 : public Element{

    int node1, node2, node3, node4, node5, node6, node7, node8;

    Iso3DHex8(int node_1, int node_2, int node_3, int node_4, int node_5, int node_6, int node_7,
              int node_8);

    DenseMatrix computeLocalStiffness() override;
};

#endif    // FEM_SRC_ENTITY_ISO2DQUAD4_H_
