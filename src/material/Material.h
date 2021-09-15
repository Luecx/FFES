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
