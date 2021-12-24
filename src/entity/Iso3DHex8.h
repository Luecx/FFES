

#ifndef FEM_SRC_ENTITY_Iso3DHex8_H_
#define FEM_SRC_ENTITY_Iso3DHex8_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"
#include "IsoElement.h"
#include "IsoElementImplementation.h"

struct Iso3DHex8 : public IsoElement<8, 3> {

    Iso3DHex8(int node_1,
              int node_2,
              int node_3,
              int node_4,
              int node_5,
              int node_6,
              int node_7,
              int node_8);

    QuickMatrix<8, 3>  getLocalShapeDerivative(Precision r, Precision s, Precision t) override;
    QuickMatrix<6, 24> computeStrainDisplacementRelationFromSource(QuickMatrix<8, 3> b_help) override;
    QuickMatrix<8, 1>  getShapeFunction(Precision r, Precision s, Precision t) override;
    DenseMatrix        getIntegrationScheme() override;
    QuickMatrix<8, 3>  getNodeLocalCoordinates() override;
};

#endif    // FEM_SRC_ENTITY_ISO2DQUAD4_H_
