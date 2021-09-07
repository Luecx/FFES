//
// Created by Luecx on 01.09.2021.
//

#ifndef FEM_SRC_SYSTEM_SYSTEM_H_
#define FEM_SRC_SYSTEM_SYSTEM_H_

#include "Step.h"


#include <vector>

class System {

    private:
//    MemoryManager      memory_manager {};
    std::vector<Step*> steps {};
};

#endif    // FEM_SRC_SYSTEM_SYSTEM_H_
