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
#ifndef FEM_SRC_ASSERT_ERROR_H_
#define FEM_SRC_ASSERT_ERROR_H_

enum ErrorCodes {
    WRONG_MATRIX_DIMENSION,
    NEGATIVE_JACOBIAN,
    NO_MATERIAL_ASSIGNED,
    MATERIAL_HAS_NO_ELASTICITY,
    ELEMENT_SET_NOT_FOUND,
    MATERIAL_NOT_FOUND,
    DIFFERENT_NUMBER_DOF_ELEMENTS,
    DUPLICATE_ELEMENT_ID,
    PARSING_SYNTAX_ERROR,
    PARSING_INVALID_NODE_ID,
    PARSING_INVALID_ELEMENT_ID,
    CANNOT_OPEN_FILE,
    ELEMENT_TYPE_NOT_KNOWN,
};

#define ERROR(expr, code, value)                                                                 \
{                                                                                                \
    if (!static_cast<bool>(expr)) {                                                              \
        std::cout << "[ERROR] in expression "              << (#expr) << std::endl;              \
        std::cout << "   value: " << value                 << std::endl;                         \
        std::cout << "    file: " << __FILE__              << std::endl;                         \
        std::cout << "    line: " << __LINE__              << std::endl;                         \
        std::cout << "    func: " << FUNCTION_NAME         << std::endl;                         \
        std::exit(code);                                                                         \
    }                                                                                            \
}

#endif    // FEM_SRC_ASSERT_ERROR_H_
