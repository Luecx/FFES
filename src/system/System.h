

#ifndef FEM_SRC_SYSTEM_SYSTEM_H_
#define FEM_SRC_SYSTEM_SYSTEM_H_

#include "LoadCase.h"

#include <vector>

struct System {

    public:
    Model model;
    std::vector<LoadCase*> load_cases {};


    public:
    System(int max_nodes, int max_elements) : model(max_nodes, max_elements){
        // add base load case
        addLoadCase<LoadCase>();
    }

    template<typename C>
    C* addLoadCase(){
        C* lc = new C(&model, getLoadCase());
        load_cases.push_back(static_cast<LoadCase*> (lc));
        return lc;
    }

    LoadCase* getLoadCase(){
        if(load_cases.size() == 0) return nullptr;
        return load_cases.back();
    }

};

#endif    // FEM_SRC_SYSTEM_SYSTEM_H_
