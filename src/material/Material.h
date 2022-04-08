

#ifndef FEM_SRC_MATERIAL_MATERIAL_H_
#define FEM_SRC_MATERIAL_MATERIAL_H_

#include <memory>
#include "../matrix/QuickMatrix.h"
#include "Elasticity.h"

struct Material {

    std::string name;
    Precision   density = 0;

    private:
    ElasticityPtr elasticity = nullptr;

    public:
    Material(const std::string& p_name) : name(p_name) {}
    Material(const Material& other) : name(other.name) {}
    Material(Material&& other) : name(other.name) {}
    Material& operator=(const Material& other){
        name = other.name;
        elasticity = other.elasticity;
        return *this;
    }
    Material& operator=(Material&& other){
        name = other.name;
        elasticity    = other.elasticity;
        return *this;
    }

    // functions to manage materials
    template<typename T, typename... Args> void setElasticity(Args&&... args) {
        this->elasticity = ElasticityPtr {new T(args...)};
    }
    ElasticityPtr getElasticity() { return elasticity; }
};

#endif    // FEM_SRC_MATERIAL_MATERIAL_H_
