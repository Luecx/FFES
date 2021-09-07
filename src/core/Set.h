//
// Created by Luecx on 05.09.2021.
//

#ifndef FEM_SRC_CORE_SET_H_
#define FEM_SRC_CORE_SET_H_

#include <string>
#include <vector>

struct Set {
    std::string name{};
    std::vector<int32_t> ids{};
};

#endif    // FEM_SRC_CORE_SET_H_
