

#ifndef FEM_SRC_ASSERT_ASSERT_H_
#define FEM_SRC_ASSERT_ASSERT_H_

#include <iostream>
#include "Logger.h"

#ifdef NDEBUG
#define ASSERT(expr)
#else
#define ASSERT(expr)                                                                                 \
    {                                                                                                \
        if (!static_cast<bool>(expr)) {                                                              \
            std::cout << "[ASSERT] in expression " << (#expr) << std::endl;                           \
            std::cout << "    file: " << __FILE__ << std::endl;                                      \
            std::cout << "    line: " << __LINE__ << std::endl;                                      \
            std::cout << "    func: " << FUNCTION_NAME << std::endl;                                 \
            std::exit(1);                                                                            \
        }                                                                                            \
    }
#endif

#endif    // FEM_SRC_ASSERT_ASSERT_H_
