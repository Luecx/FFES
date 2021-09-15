/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFESis free software: you can redistribute it and/or modify                   *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFESis distributed in the hope that it will be useful,                     *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

#ifndef FEM_SRC_ENTITY_MODEL_H_
#define FEM_SRC_ENTITY_MODEL_H_

#include "../assert/Error.h"
#include "../assert/Warning.h"
#include "../core/NodeData.h"
#include "../core/Set.h"
#include "../entity/Element.h"
#include "../material/Material.h"
#include "../eigen/SparseCore"

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

    ID                     active_node_set = -1;
    ID                     active_element_set = -1;

    Model(int p_node_count, int p_element_count)
        : max_node_count   (p_node_count),
          max_element_count(p_element_count) {

        node_data[USED                               ].init(max_node_count * 1, max_node_count).even(1);
        node_data[POSITION                           ].init(max_node_count * 3, max_node_count).even(3);
        node_data[BOUNDARY_IS_CONSTRAINED            ].init(max_node_count * 3, max_node_count).even(3);
        node_data[BOUNDARY_DISPLACEMENT              ].init(max_node_count * 3, max_node_count).even(3);
        node_data[BOUNDARY_FORCE                     ].init(max_node_count * 3, max_node_count).even(3);
        node_data[BOUNDARY_IMPLIED_DISPLACEMENT_FORCE].init(max_node_count * 3, max_node_count).even(3);
        node_data[DISPLACEMENT                       ].init(max_node_count * 3, max_node_count).even(3);
        node_data[STRESS                             ].init(max_node_count * 6, max_node_count).even(6);

        elements.resize(max_element_count);
        for(int i = 0; i < max_element_count; i++) elements[i] = nullptr;

        activateElementSet("EALL");
        activateNodeSet("NALL");
    }

    virtual ~Model() {
        for(Element*& element:elements){
            delete element;
            element = nullptr;
        }
        for(Material*& mat:materials){
            delete mat;
            mat = nullptr;
        }
    }

    // functions to add nodes/elements
    void setNode(ID id, Precision x, Precision y, Precision z=(Precision)0){

        ERROR(id < max_node_count, PARSING_INVALID_NODE_ID, "id is larger than max_node_count");

        node_data[POSITION][id][0] = x;
        node_data[POSITION][id][1] = y;
        node_data[POSITION][id][2] = z;

        this->node_sets[active_node_set].ids.push_back(id);
    }
    template<typename T, typename... Args>
    void setElement(ID id, Args&&... args){
        auto el = new T{args...};
        if(nodal_dimension == 0){
            nodal_dimension = el->nodeDOF();
        }
        ERROR(nodal_dimension == el->nodeDOF(), DIFFERENT_NUMBER_DOF_ELEMENTS, nodal_dimension);
        ERROR(this->elements[id] == nullptr, DUPLICATE_ELEMENT_ID, id)

        this->elements[id] = el;
        this->elements[id]->node_data = &node_data;

        this->element_sets[active_element_set].ids.push_back(id);

        for(int i = 0; i < static_cast<Element*>(el)->nodeCount(); i++){
            node_data[USED][static_cast<Element*>(el)->nodeIDS()[i]] = 1;
        }
    }

    // functions to manage materials
    ID addMaterial(const std::string& name){
        this->materials.push_back(new Material(name));
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
    ID activateNodeSet(const std::string& name);
    ID activateElementSet(const std::string& name);
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
    Eigen::SparseMatrix<Precision> buildReducedStiffnessMatrix();

    // build load vector
    Eigen::VectorXd buildReducedLoadVector();

    // compute displacements for every node
    void postProcessDisplacements(const Eigen::VectorXd& displacement);

    // numerate unconstrained vertices
    ID numerateUnconstrainedNodes();
};


#endif    // FEM_SRC_ENTITY_MODEL_H_
