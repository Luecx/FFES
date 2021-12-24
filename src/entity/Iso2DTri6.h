

#ifndef FEM_SRC_ENTITY_ISO2DTRI6_H_
#define FEM_SRC_ENTITY_ISO2DTRI6_H_

#include "../matrix/Inverse.h"
#include "../matrix/QuickMatrix.h"
#include "Element.h"
#include "IsoElement.h"
#include "IsoElementImplementation.h"

struct Iso2DTri6 : public IsoElement<6, 2> {

    Iso2DTri6(int node_1, int node_2, int node_3,int node_4, int node_5, int node_6);

    QuickMatrix<6, 1>  getShapeFunction(Precision r, Precision s, Precision t) override;
    QuickMatrix<6, 2>  getLocalShapeDerivative(Precision r, Precision s, Precision t) override;
    QuickMatrix<3, 12> computeStrainDisplacementRelationFromSource(QuickMatrix<6, 2> b_help) override;
    DenseMatrix        getIntegrationScheme() override;
    QuickMatrix<6, 2>  getNodeLocalCoordinates() override;

    //    QuickMatrix<2, 3> getLocalShapeDerivative(Precision r, Precision s, Precision t) override;
    //    QuickMatrix<3, 6> computeStrainDisplacementRelationFromSource(QuickMatrix<2, 3> b_help)
    //    override; DenseMatrix       getIntegrationScheme() override; QuickMatrix<3, 1>
    //    getShapeFunction(Precision r, Precision s, Precision t) override; QuickMatrix<3, 2>
    //    getNodeLocalCoordinates() override;
};

#endif    // FEM_SRC_ENTITY_ISO2DTRI3_H_
