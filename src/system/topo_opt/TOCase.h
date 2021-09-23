

#ifndef FEM_SRC_SYSTEM_TOPO_OPT_TOCASE_H_
#define FEM_SRC_SYSTEM_TOPO_OPT_TOCASE_H_

#include "../LoadCase.h"

struct TOCase : public LoadCase{

    TOCase(Model* p_model, LoadCase* p_previous, int iterations, Precision min_res) : LoadCase(p_model, p_previous) {
        this->element_data[SIMP_DENSITY_FACTOR]
            .init(model->max_element_count, model->max_element_count)
            .even()
            .fill(1);
        this->element_data[SIMP_DENSITY_EXPONENT]
            .init(model->max_element_count, model->max_element_count)
            .even()
            .fill(3);
    }

    virtual ~TOCase() {
        this->element_data[SIMP_DENSITY_EXPONENT].cleanUp();
        this->element_data[SIMP_DENSITY_FACTOR  ].cleanUp();
    }
};

#endif    // FEM_SRC_SYSTEM_TOPO_OPT_TOCASE_H_
