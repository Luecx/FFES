//
// Created by Luecx on 05.09.2021.
//

#ifndef FEM_SRC_CORE_DEFS_H_
#define FEM_SRC_CORE_DEFS_H_

#include "stdint.h"

typedef int32_t ID;
#define DOUBLE_PRECISION
#ifdef DOUBLE_PRECISION
typedef double Precision;
#else
typedef float Precision;
#endif

#define NONE (Precision)(NAN)

#endif    // FEM_SRC_CORE_DEFS_H_
