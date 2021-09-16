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
        addLoadCase();
    }

    void addLoadCase(){
        load_cases.push_back(new LoadCase(&model, getLoadCase()));
    }

    LoadCase* getLoadCase(){
        if(load_cases.size() == 0) return nullptr;
        return load_cases.back();
    }

};

#endif    // FEM_SRC_SYSTEM_SYSTEM_H_
