

#ifndef FEM_SRC_MATERIAL_ISOTROPICELASTICITY_H_
#define FEM_SRC_MATERIAL_ISOTROPICELASTICITY_H_

#include "Elasticity.h"
#include "Material.h"

struct IsotropicElasticity : Elasticity {

    private:

    Precision youngs_module;
    Precision poisson;

    public:
    IsotropicElasticity(Precision youngs_module, Precision poisson);

    private:
    void update() override;


};

#endif    // FEM_SRC_MATERIAL_ISOTROPICELASTICITY_H_
