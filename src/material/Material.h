

#ifndef FEM_SRC_MATERIAL_MATERIAL_H_
#define FEM_SRC_MATERIAL_MATERIAL_H_

#include "../matrix/QuickMatrix.h"
#include "Elasticity.h"

struct Material {

    std::string name;
    Precision   density = 0;


    private:
    bool is_copy           = false;
    Elasticity* elasticity = nullptr;

    public:
    Material(const std::string& p_name) : name(p_name) {}
    Material(const Material& other) : name(other.name) {
        is_copy       = true;
        elasticity    = other.elasticity;
    }
    Material(Material&& other) : name(other.name) {
        is_copy       = other.is_copy;
        other.is_copy = true;
        elasticity    = other.elasticity;
    }
    Material& operator=(const Material& other) = delete;
    Material& operator=(Material&& other) = delete;
//    Material& operator=(const Material& other){
//        if (!is_copy && elasticity != nullptr) {
//            delete elasticity;
//        }
//        is_copy    = true;
//        elasticity = other.elasticity;
//        return *this;
//    }
//    Material& operator=(Material&& other){
//        if (!is_copy && elasticity != nullptr) {
//            delete elasticity;
//        }
//        is_copy       = other.is_copy;
//        other.is_copy = true;
//        elasticity    = other.elasticity;
//        return *this;
//    }

    // functions to manage materials
    template<typename T, typename... Args> void setElasticity(Args&&... args) {
        this->elasticity = new T(args...);
    }
    Elasticity* getElasticity() { return elasticity; }

    virtual ~Material() {
        if (!is_copy && elasticity != nullptr) {
            delete elasticity;
        }
        elasticity = nullptr;
    }
};

#endif    // FEM_SRC_MATERIAL_MATERIAL_H_
