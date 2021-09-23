
#ifndef FEM_SRC_ASSERT_LOGGER_H_
#define FEM_SRC_ASSERT_LOGGER_H_

#if defined(__GNUC__) || defined(__clang__)
#define FUNCTION_NAME __PRETTY_FUNCTION__
#elif _MSC_VER
#define FUNCTION_NAME __FUNCSIG__
#else
#define FUNCTION_NAME __func__
#endif





#endif    // FEM_SRC_ASSERT_LOGGER_H_
