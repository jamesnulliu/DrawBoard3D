#pragma once
#include <iostream>
#include <format>

#define DB3D_RESET     "\033[0m"
#define DB3D_RED       "\033[1;31m"
#define DB3D_GREEN     "\033[1;32m"
#define DB3D_YELLOW    "\033[1;33m"
#define DB3D_BLUE      "\033[1;34m"
#define DB3D_MAGENTA   "\033[1;35m"
#define DB3D_CYAN      "\033[1;36m"
#define DB3D_WHITE     "\033[1;37m"

#define DB3D_LOG_FMT   "[{}:Line{}:{}]>>>>>\n+ {}\n"

#define DB3D_TRACE(...)  \
    std::cout << std::format(DB3D_LOG_FMT, __FILE__, __LINE__, __FUNCTION__, std::format(__VA_ARGS__))

#define DB3D_INFO(...)  \
    std::cout << std::format(DB3D_GREEN DB3D_LOG_FMT DB3D_RESET, __FUNCTION__, __FILE__, __LINE__, std::format(__VA_ARGS__))

#define DB3D_WARNING(...)  \
    std::cout << std::format(DB3D_YELLOW DB3D_LOG_FMT DB3D_RESET, __FUNCTION__, __FILE__, __LINE__, std::format(__VA_ARGS__))

#define DB3D_ERROR(...)  \
    std::cout << std::format(DB3D_RED DB3D_LOG_FMT DB3D_RESET, __FUNCTION__, __FILE__, __LINE__, std::format(__VA_ARGS__))

#define DB3D_CRITICAL(...)  \
    std::cout << std::format(DB3D_CYAN DB3D_LOG_FMT DB3D_RESET, __FUNCTION__, __FILE__, __LINE__, std::format(__VA_ARGS__))
