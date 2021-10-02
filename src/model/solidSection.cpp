
#include "Model.h"

void Model::solidSection(const std::string& set, const std::string& material, Precision thickness) {
    ID set_id = getElementSetID(set);
    ID mat_id = getMaterialID(material);

    WARNING(set_id >= 0, "did not find element set: " << set);
    WARNING(mat_id >= 0, "did not find material: " << material);

    if(set_id < 0 || mat_id < 0) return;

    for(auto id:element_sets[set_id].ids){
        elements[id]->material = materials[mat_id];
        element_data[ELEMENT_THICKNESS][id][0] = thickness;
    }
}

