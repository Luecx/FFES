
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
