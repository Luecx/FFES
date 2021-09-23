

#include "Model.h"

ID Model::addMaterial(const std::string& name) {
    this->materials.push_back(new Material(name));
    return this->materials.size()-1;
}
ID Model::getMaterialID(const std::string& name) {
    for(int i = 0; i < (int)materials.size(); i++){
        if(materials[i]->name == name){
            return i;
        }
    }
    return -1;
}