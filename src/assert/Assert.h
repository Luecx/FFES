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
