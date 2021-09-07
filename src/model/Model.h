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

    int                    node_count    = 0;
    int                    element_count = 0;

    ID                     active_node_set = -1;
    ID                     active_element_set = -1;

    Model(int p_node_count, int p_element_count)
        : max_node_count   (p_node_count),
          max_element_count(p_element_count) {

        node_data[POSITION].init(max_node_count * 3, max_node_count);
        elements.reserve(max_element_count);

        activeElementSet("EALL");
        activeNodeSet("NALL");
    }

    virtual ~Model() {
        for(Element* element:elements){
            delete[] element;
        }

        for(Material* mat:materials){
            delete mat;
        }
    }

    // functions to add nodes/elements
    void addNode(Precision x, Precision y, Precision z=0){
        node_data[POSITION].setIndexIncremental(node_count++, 3);
        node_data[POSITION][node_count-1][0] = x;
        node_data[POSITION][node_count-1][1] = y;
        node_data[POSITION][node_count-1][2] = z;

        this->node_sets[active_node_set].ids.push_back(node_count-1);
    }
    template<typename T, typename... Args>
    void addElement(Args&&... args){
        this->elements[element_count++] = new T{args...};
        this->elements[element_count-1]->node_data = &node_data;

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
    ID activeNodeSet(const std::string& name){
        ID id = getNodeSetID(name);
        if(id == -1){
            this->node_sets.push_back(Set{name});
            this->active_node_set = this->node_sets.size()-1;
            return this->active_node_set;
        }else{
            this->active_node_set = id;
            return id;
        }
    }
    ID activeElementSet(const std::string& name){
        ID id = getElementSetID(name);
        if(id == -1){
            this->element_sets.push_back(Set{name});
            this->active_element_set = this->element_sets.size()-1;
            return this->active_element_set;
        }else{
            this->active_element_set = id;
            return id;
        }
    }
    ID getNodeSetID(const std::string& name){
        for(auto i = 0; i < (int)node_sets.size(); i++){
            if(node_sets[i].name == name){
                return i;}
        }
        return -1;
    }
    ID getElementSetID(const std::string& name){
        for(auto i = 0; i < (int)element_sets.size(); i++){
            if(element_sets[i].name == name){
                return i;}
        }
        return -1;
    }

    // assign materials to elements
    void solidSection(const std::string& set, const std::string& material);

    // build global stiffness matrix
    void buildReducedStiffnessMatrix();

    // numerate unconstrained vertices
    void numerateUnconstrainedNodes();
};


#endif    // FEM_SRC_ENTITY_MODEL_H_
