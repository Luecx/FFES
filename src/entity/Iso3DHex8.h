

#ifndef FEM_SRC_ENTITY_Iso3DHex8_H_
#define FEM_SRC_ENTITY_Iso3DHex8_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"

struct Iso3DHex8 : public Element{

    int node_ids[8]{};

    Iso3DHex8(int node_1, int node_2, int node_3, int node_4, int node_5, int node_6, int node_7,
              int node_8);

    DenseMatrix computeLocalStiffness() override;
    DenseMatrix computeStress(LoadCase* load_case, const DenseMatrix& evaluation_points) override;
    DenseMatrix getIntegrationScheme() override;

    QuickMatrix<6,24> computeStrainDisplacementRelation(QuickMatrix<8,3> &node_coords,
                                                         Precision r,
                                                         Precision s,
                                                         Precision t,
                                                         Precision &determinant);

    int         nodeDOF() override;
    int         nodeCount() override;
    int*        nodeIDS() override;
};

#endif    // FEM_SRC_ENTITY_ISO2DQUAD4_H_
