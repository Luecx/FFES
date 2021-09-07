//
// Created by Luecx on 07.09.2021.
//
#include "Model.h"

void Model::solidSection(const std::string& set, const std::string& material) {
    ID set_id = getElementSetID(set);
    ID mat_id = getMaterialID(material);

    ERROR(set_id >= 0, ELEMENT_SET_NOT_FOUND);
    ERROR(mat_id >= 0, MATERIAL_NOT_FOUND);

    for(auto id:element_sets[set_id].ids){
        elements[id]->material = materials[mat_id];
    }
}

