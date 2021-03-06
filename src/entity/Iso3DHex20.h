

#ifndef FEM_SRC_ENTITY_Iso3DHex20_H_
#define FEM_SRC_ENTITY_Iso3DHex20_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"
#include "IsoElement.h"
#include "IsoElementImplementation.h"

struct Iso3DHex20 : public IsoElement<20, 3> {

    Iso3DHex20(int node_1,
               int node_2,
               int node_3,
               int node_4,
               int node_5,
               int node_6,
               int node_7,
               int node_8,
               int node_9,
               int node_10,
               int node_11,
               int node_12,
               int node_13,
               int node_14,
               int node_15,
               int node_16,
               int node_17,
               int node_18,
               int node_19,
               int node_20);

    QuickMatrix<20, 3> getLocalShapeDerivative(Precision r, Precision s, Precision t) override;
    QuickMatrix<6, 60> computeStrainDisplacementRelationFromSource(QuickMatrix<20, 3> b_help) override;
    QuickMatrix<20, 1> getShapeFunction(Precision r, Precision s, Precision t) override;

    DenseMatrix        getIntegrationScheme() override;
    QuickMatrix<20, 3> getNodeLocalCoordinates() override;
};

#endif    // FEM_SRC_ENTITY_Iso3DHex20_H_
