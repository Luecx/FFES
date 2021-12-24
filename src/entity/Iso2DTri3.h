

#ifndef FEM_SRC_ENTITY_ISO2DTRI3_H_
#define FEM_SRC_ENTITY_ISO2DTRI3_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"
#include "IsoElement.h"
#include "IsoElementImplementation.h"

struct Iso2DTri3 : public IsoElement<3, 2> {

    Iso2DTri3(int node_1, int node_2, int node_3);
    QuickMatrix<3, 2> getLocalShapeDerivative(Precision r, Precision s, Precision t) override;
    QuickMatrix<3, 6> computeStrainDisplacementRelationFromSource(QuickMatrix<3, 2> b_help) override;
    DenseMatrix       getIntegrationScheme() override;
    QuickMatrix<3, 1> getShapeFunction(Precision r, Precision s, Precision t) override;
    QuickMatrix<3, 2> getNodeLocalCoordinates() override;
};

#endif    // FEM_SRC_ENTITY_ISO2DTRI3_H_
