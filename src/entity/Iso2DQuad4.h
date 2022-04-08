

#ifndef FEM_SRC_ENTITY_ISO2DQUAD4_H_
#define FEM_SRC_ENTITY_ISO2DQUAD4_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"
#include "IsoElement.h"
#include "IsoElementImplementation.h"

struct Iso2DQuad4 : public IsoElement<4, 2> {

    Iso2DQuad4(int node_1, int node_2, int node_3, int node_4);

    QuickMatrix<4, 2> getLocalShapeDerivative(Precision r, Precision s, Precision t) override;
    QuickMatrix<3, 8> computeStrainDisplacementRelationFromSource(QuickMatrix<4, 2> b_help) override;
    DenseMatrix       getIntegrationScheme() override;
    QuickMatrix<4, 1> getShapeFunction(Precision r, Precision s, Precision t) override;
    QuickMatrix<4, 2> getNodeLocalCoordinates() override;

};

#endif    // FEM_SRC_ENTITY_ISO2DQUAD4_H_
