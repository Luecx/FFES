//
// Created by Luecx on 05.09.2021.
//

#ifndef FEM_SRC_ASSERT_ERROR_H_
#define FEM_SRC_ASSERT_ERROR_H_

enum ErrorCodes {
    WRONG_MATRIX_DIMENSION,
    NEGATIVE_JACOBIAN,
    NO_MATERIAL_ASSIGNED,
    ELEMENT_SET_NOT_FOUND,
    MATERIAL_NOT_FOUND
};

#define ERROR(expr, code)                                                                            \
{                                                                                                \
    if (!static_cast<bool>(expr)) {                                                              \
        std::cout << "[ERROR] in expression " << (#expr) << std::endl;                           \
        std::cout << "    file: " << __FILE__ << std::endl;                                      \
        std::cout << "    line: " << __LINE__ << std::endl;                                      \
        std::cout << "    func: " << FUNCTION_NAME << std::endl;                                 \
        std::exit(code);                                                                         \
    }                                                                                            \
}

#endif    // FEM_SRC_ASSERT_ERROR_H_
