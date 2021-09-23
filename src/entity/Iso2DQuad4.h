

#ifndef FEM_SRC_ENTITY_ISO2DQUAD4_H_
#define FEM_SRC_ENTITY_ISO2DQUAD4_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"

struct Iso2DQuad4 : public Element {

    int node_ids[4] {};
    Iso2DQuad4(int node_1, int node_2, int node_3, int node_4);

    DenseMatrix computeLocalStiffness() override;
    DenseMatrix computeStress(LoadCase* load_case, const DenseMatrix& evaluation_points) override;
    DenseMatrix getIntegrationScheme() override;

    int         nodeDOF() override;
    int         nodeCount() override;
    int*        nodeIDS() override;
};

#endif    // FEM_SRC_ENTITY_ISO2DQUAD4_H_
