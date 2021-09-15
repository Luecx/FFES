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
//
// Created by Luecx on 15.09.2021.
//

#ifndef FEM_SRC_ASSERT_WARNING_H_
#define FEM_SRC_ASSERT_WARNING_H_

#define WARNING(expr, value)                                                                 \
{                                                                                                \
    if (!static_cast<bool>(expr)) {                                                              \
        std::cout << "[WARNING] in expression "            << (#expr) << std::endl;              \
        std::cout << "   value: " << value                 << std::endl;                         \
        std::cout << "    file: " << __FILE__              << std::endl;                         \
        std::cout << "    line: " << __LINE__              << std::endl;                         \
        std::cout << "    func: " << FUNCTION_NAME         << std::endl;                         \
    }                                                                                            \
}

#endif    // FEM_SRC_ASSERT_WARNING_H_
