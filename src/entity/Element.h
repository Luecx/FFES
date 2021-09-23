

#ifndef FEM_SRC_ENTITY_ELEMENT_H_
#define FEM_SRC_ENTITY_ELEMENT_H_

#include "../core/NodeData.h"
#include "../material/Material.h"
#include "../matrix/DenseMatrix.h"

struct LoadCase;

struct Element {

    NodeData* node_data = nullptr;
    Material* material = nullptr;

    virtual ~Element() {}

    virtual DenseMatrix computeLocalStiffness() = 0;
    virtual DenseMatrix computeStress(LoadCase* load_case, const DenseMatrix& evaluation_points) = 0;
    virtual DenseMatrix getIntegrationScheme() = 0;

    virtual int  nodeDOF() = 0;
    virtual int  nodeCount() = 0;
    virtual int* nodeIDS() = 0;
};


#endif    // FEM_SRC_ENTITY_ELEMENT_H_
