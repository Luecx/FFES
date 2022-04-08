

#ifndef FEM_SRC_ENTITY_ELEMENT_H_
#define FEM_SRC_ENTITY_ELEMENT_H_

#include "../core/NodeData.h"
#include "../material/Material.h"
#include "../matrix/DenseMatrix.h"
#include <memory>

struct LoadCase;
struct Model;

struct Element {

    Model*    model      = nullptr;
    Material* material   = nullptr;
    ID        element_id = 0;

    virtual ~Element() {}

    virtual DenseMatrix computeLocalStiffness(LoadCase* load_case)                                = 0;
    virtual DenseMatrix computeLocalMassMatrix(LoadCase* load_case)                               = 0;
    virtual DenseMatrix computeStressAtNodes(LoadCase* load_case)                                 = 0;

    virtual DenseMatrix getNodalData(NodeDataEntries entry, NodeData* p_node_data, int dim_count) = 0;
    virtual DenseMatrix getNodalData(NodeDataEntries entry, LoadCase* load_case, int dim_count)   = 0;
    virtual DenseMatrix getNodalData(NodeDataEntries entry, int dim_count)                        = 0;

    virtual Precision   interpolate(DenseMatrix nodal, Precision r, Precision s, Precision t = 0) = 0;
    virtual Precision   compliance(LoadCase* load_case)                                           = 0;

    virtual int         nodeDOF()                                                                 = 0;
    virtual int         nodeCount()                                                               = 0;
    virtual int*        nodeIDS()                                                                 = 0;
};

using ElementPtr = std::shared_ptr<Element>;

#endif    // FEM_SRC_ENTITY_ELEMENT_H_
