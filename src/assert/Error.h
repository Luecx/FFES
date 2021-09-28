
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
    NOT_YET_SUPPORTED,
    UNINITIALISED,
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
