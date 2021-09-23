

#ifndef FEM_SRC_CORE_SET_H_
#define FEM_SRC_CORE_SET_H_

#include "defs.h"

#include <ostream>
#include <string>
#include <vector>

struct Set {
    std::string name{};
    std::vector<ID> ids{};

    friend std::ostream& operator<<(std::ostream& os, const Set& set);
};

#endif    // FEM_SRC_CORE_SET_H_
