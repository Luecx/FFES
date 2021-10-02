

#ifndef FEM_SRC_ENTITY_Iso2DQuad8_H_
#define FEM_SRC_ENTITY_Iso2DQuad8_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"
#include "IsoElement.h"
#include "IsoElementImplementation.h"

struct Iso2DQuad8 : public IsoElement<8, 2> {

    Iso2DQuad8(int node_1, int node_2, int node_3, int node_4, int node_5, int node_6, int node_7, int node_8);

    QuickMatrix<2, 8>  getLocalShapeDerivative(Precision r, Precision s, Precision t) override;
    QuickMatrix<3, 16> computeStrainDisplacementRelationFromSource(QuickMatrix<2, 8> b_help) override;
    DenseMatrix        getIntegrationScheme() override;

    //    QuickMatrix<2, 4> getLocalShapeDerivative(Precision r, Precision s, Precision t) override;
//    QuickMatrix<3, 8> computeStrainDisplacementRelationFromSource(QuickMatrix<2, 4> b_help) override;
//    DenseMatrix       getIntegrationScheme() override;

    //    DenseMatrix computeLocalStiffness(LoadCase* load_case) override;
    //    DenseMatrix computeStress(LoadCase* load_case, const DenseMatrix& evaluation_points)
    //    override; DenseMatrix getIntegrationScheme() override;
    //
    //    int         nodeDOF() override;
    //    int         nodeCount() override;
    //    int*        nodeIDS() override;
};

#endif    // FEM_SRC_ENTITY_Iso2DQuad8_H_
