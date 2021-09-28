
//
// Created by Luecx on 25.09.2021.
//
#include "LoadCase.h"

void LoadCase::simp(const std::string& set, Precision rho, Precision p, bool isTemp) {
    ID element_set_id = model->getElementSetID(set);
    ASSERT(element_set_id >= 0);

    for(ID id:model->element_sets[element_set_id].ids){
        simp(id, rho, p, isTemp);
    }
}
void LoadCase::simp(int element_id, Precision rho, Precision p, bool isTemp) {
    if(!element_data[SIMP_DENSITY_EXPONENT].isInitialised()){
        element_data[SIMP_DENSITY_EXPONENT]
            .init(model->max_element_count, model->max_node_count)
            .even(1)
            .fill(1);
    }
    if(!element_data[SIMP_DENSITY_FACTOR].isInitialised()){
        element_data[SIMP_DENSITY_FACTOR]
            .init(model->max_element_count, model->max_node_count)
            .even(1)
            .fill(1);
    }

    element_data[SIMP_DENSITY_FACTOR  ].set(element_id, 0, rho, !isTemp);
    element_data[SIMP_DENSITY_EXPONENT].set(element_id, 0, p  , !isTemp);

}

