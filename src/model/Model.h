//
// Created by Luecx on 02.09.2021.
//

#ifndef FEM_SRC_ENTITY_MODEL_H_
#define FEM_SRC_ENTITY_MODEL_H_

#include "../assert/Error.h"
#include "../core/NodeData.h"
#include "../core/Set.h"
#include "../entity/Element.h"
#include "../material/Material.h"
#include "../matrix/ListedMatrix.h"

#include <vector>

struct Model {

    NodeData               node_data {};
    std::vector<Element*>  elements {};
    std::vector<Material*> materials {};

    // list of sets for elements and nodes
    std::vector<Set>       node_sets {};
    std::vector<Set>       element_sets {};

    // constants for this model
    const int              max_node_count;
    const int              max_element_count;

    int                    nodal_dimension = 0;
    int                    node_count    = 0;
    int                    element_count = 0;

    ID                     active_node_set = -1;
    ID                     active_element_set = -1;

    Model(int p_node_count, int p_element_count)
        : max_node_count   (p_node_count),
          max_element_count(p_element_count) {

        node_data[POSITION].init(max_node_count * 3, max_node_count);
        node_data[BOUNDARY_IS_CONSTRAINED].init(max_node_count * 3, max_node_count);
        node_data[BOUNDARY_DISPLACEMENT  ].init(max_node_count * 3, max_node_count);
        node_data[BOUNDARY_FORCE         ].init(max_node_count * 3, max_node_count);
        elements.reserve(max_element_count);

        activeElementSet("EALL");
        activeNodeSet("NALL");
    }

    virtual ~Model() {
        for(Element* element:elements){
            delete element;
        }

        for(Material* mat:materials){
            delete mat;
        }
    }

    // functions to add nodes/elements
    void addNode(Precision x, Precision y, Precision z=(Precision)0){
        node_data[POSITION].setIndexIncremental(node_count++, 3);
        node_data[POSITION][node_count-1][0] = x;
        node_data[POSITION][node_count-1][1] = y;
        node_data[POSITION][node_count-1][2] = z;
        node_data[BOUNDARY_IS_CONSTRAINED].setIndexIncremental(node_count-1, 3);
        node_data[BOUNDARY_DISPLACEMENT  ].setIndexIncremental(node_count-1, 3);
        node_data[BOUNDARY_FORCE         ].setIndexIncremental(node_count-1, 3);

        this->node_sets[active_node_set].ids.push_back(node_count-1);
    }
    template<typename T, typename... Args>
    void addElement(Args&&... args){
        auto el = new T{args...};
        if(nodal_dimension == 0){
            nodal_dimension = el->nodeDOF();
        }
        ERROR(nodal_dimension == el->nodeDOF(), DIFFERENT_NUMBER_DOF_ELEMENTS)
        this->elements.push_back(el);
        this->elements[element_count++]->node_data = &node_data;

        this->element_sets[active_element_set].ids.push_back(element_count-1);
    }

    // functions to manage materials
    template<typename T, typename... Args>
    ID addMaterial(const std::string& name, Args&&... args){
        Material* mat  = new T{args...};
        mat->name = name;
        this->materials.push_back(mat);
        return this->materials.size()-1;
    }
    ID getMaterialID(const std::string& name){
        for(int i = 0; i < (int)materials.size(); i++){
            if(materials[i]->name == name){
                return i;
            }
        }
        return -1;
    }

    // functions to manage sets
    ID activeNodeSet(const std::string& name);
    ID activeElementSet(const std::string& name);
    ID getNodeSetID(const std::string& name);
    ID getElementSetID(const std::string& name);

    // functions to constraint nodes
    void constraint(const std::string& set,
                    Precision x=NAN,
                    Precision y=NAN,
                    Precision z=NAN);
    void constraint(int node_id,
                    Precision x=NAN,
                    Precision y=NAN,
                    Precision z=NAN);

    // functions to apply loads to nodes
    void applyLoad(const std::string& set,
                    Precision x=NAN,
                    Precision y=NAN,
                    Precision z=NAN);
    void applyLoad(int node_id,
                    Precision x=NAN,
                    Precision y=NAN,
                    Precision z=NAN);

    // assign materials to elements
    void solidSection(const std::string& set, const std::string& material);

    // build global stiffness matrix
    ListedMatrix buildReducedStiffnessMatrix();

    // build load vector
    DenseMatrix buildReducedLoadVector(const ListedMatrix& reducedStiffness);

    // numerate unconstrained vertices
    ID numerateUnconstrainedNodes();
};


#endif    // FEM_SRC_ENTITY_MODEL_H_
