

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
    QuickMatrix<8, 1>  getShapeFunction(Precision r, Precision s, Precision t) override;
    QuickMatrix<8, 2>  getNodeLocalCoordinates() override;
};

#endif    // FEM_SRC_ENTITY_Iso2DQuad8_H_
