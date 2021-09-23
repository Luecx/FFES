

#ifndef FEM_SRC_CORE_ELEMENTDATA_H_
#define FEM_SRC_CORE_ELEMENTDATA_H_

#include "ComponentContainer.h"
#include "defs.h"

#include <ostream>

enum ElementDataEntries{
    SIMP_DENSITY_FACTOR,
    SIMP_DENSITY_EXPONENT,
    N_MAX_ELEMENT_FLOAT_ENTRIES
};

enum ElementDataIntegerEntries{
    N_MAX_ELEMENT_INT_ENTRIES
};

struct ElementData{
    ComponentContainer<Precision> float_data[N_MAX_ELEMENT_FLOAT_ENTRIES]{};
    ComponentContainer<int>       int_data  [N_MAX_ELEMENT_INT_ENTRIES]{};

    ComponentContainer<Precision>& operator[](ElementDataEntries entry){
        return float_data[entry];
    }

    ComponentContainer<int>&       operator[](ElementDataIntegerEntries entry){
        return int_data[entry];
    }

};

#endif    // FEM_SRC_CORE_NODEDATA_H_
