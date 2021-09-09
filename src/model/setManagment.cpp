//
// Created by Luecx on 08.09.2021.
//
#include "Model.h"

ID Model::activeNodeSet(const std::string& name) {
    ID id = getNodeSetID(name);
    if(id == -1){
        this->node_sets.push_back(Set{name});
        this->active_node_set = this->node_sets.size()-1;
        return this->active_node_set;
    }else{
        this->active_node_set = id;
        return id;
    }
}
ID Model::activeElementSet(const std::string& name) {
    ID id = getElementSetID(name);
    if(id == -1){
        this->element_sets.push_back(Set{name});
        this->active_element_set = this->element_sets.size()-1;
        return this->active_element_set;
    }else{
        this->active_element_set = id;
        return id;
    }
}
ID Model::getNodeSetID(const std::string& name) {
    for(auto i = 0; i < (int)node_sets.size(); i++){
        if(node_sets[i].name == name){
            return i;}
    }
    return -1;
}
ID Model::getElementSetID(const std::string& name) {
    for(auto i = 0; i < (int)element_sets.size(); i++){
        if(element_sets[i].name == name){
            return i;}
    }
    return -1;
}