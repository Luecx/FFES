

#ifndef FEM_SRC_INTEGRATION_EXTRAPOLATE_H_
#define FEM_SRC_INTEGRATION_EXTRAPOLATE_H_

#include "Quadrature.h"

template<Domain domain, Order order>
DenseMatrix extrapolate(){
    if constexpr (domain == ISO_TRI){

        if constexpr (order == LINEAR){
            return DenseMatrix{3,1,
                1,
                1,
                1};
        }

        if constexpr (order == QUADRATIC){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }

        if constexpr (order == CUBIC){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }
    }

    if constexpr (domain == ISO_QUAD){

        if constexpr (order == LINEAR){
            return DenseMatrix{4,4,
                1 + 0.5 * sqrt(3),   - 0.5          , 1 - 0.5 * sqrt(3),   - 0.5,
                  - 0.5          , 1 + 0.5 * sqrt(3),   - 0.5          , 1 - 0.5 * sqrt(3),
                1 - 0.5 * sqrt(3),   - 0.5          , 1 + 0.5 * sqrt(3),   - 0.5,
                  - 0.5          , 1 - 0.5 * sqrt(3),   - 0.5          , 1 + 0.5 * sqrt(3)};
        }

        if constexpr (order == QUADRATIC){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }
    }


    if constexpr (domain == ISO_HEX){

        if constexpr (order == LINEAR){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }

        if constexpr (order == QUADRATIC){
            ERROR(false, NOT_YET_SUPPORTED, "not yet supported");
        }
    }
}

#endif    // FEM_SRC_INTEGRATION_EXTRAPOLATE_H_
