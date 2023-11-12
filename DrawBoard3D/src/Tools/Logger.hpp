#pragma once
#include <iostream>
#include <format>
#include <source_location>

namespace db3d {
    constexpr std::string_view SrcName(std::source_location location = std::source_location::current()) {
        std::string_view s = location.file_name();
        if constexpr (_WIN32 || _WIN64) {
            return s.substr(s.find_last_of('\\') + 1);
        } else {
            return s.substr(s.find_last_of('/') + 1);
        }
    }
}

#define DB3D_LOG_RESET     "\033[0m"
#define DB3D_LOG_RED       "\033[1;31m"
#define DB3D_LOG_GREEN     "\033[1;32m"
#define DB3D_LOG_YELLOW    "\033[1;33m"
#define DB3D_LOG_BLUE      "\033[1;34m"
#define DB3D_LOG_MAGENTA   "\033[1;35m"
#define DB3D_LOG_CYAN      "\033[1;36m"
#define DB3D_LOG_WHITE     "\033[1;37m"

#define DB3D_LOG_FMT   "[{}:{}|{}]>>>>>\n+ {}\n"

#define DB3D_TRACE(...)  \
    std::cout << std::format(DB3D_LOG_FMT, db3d::SrcName(), __LINE__, __FUNCTION__, std::format(__VA_ARGS__))

#define DB3D_INFO(...)  \
    std::cout << std::format(DB3D_LOG_GREEN DB3D_LOG_FMT DB3D_LOG_RESET, db3d::SrcName(), __LINE__, __FUNCTION__, std::format(__VA_ARGS__))

#define DB3D_WARNING(...)  \
    std::cout << std::format(DB3D_LOG_YELLOW DB3D_LOG_FMT DB3D_LOG_RESET, db3d::SrcName(), __LINE__, __FUNCTION__, std::format(__VA_ARGS__))

#define DB3D_ERROR(...)  \
    std::cout << std::format(DB3D_LOG_RED DB3D_LOG_FMT DB3D_LOG_RESET, db3d::SrcName(), __LINE__, __FUNCTION__, std::format(__VA_ARGS__))

#define DB3D_CRITICAL(...)  \
    std::cout << std::format(DB3D_LOG_CYAN DB3D_LOG_FMT DB3D_LOG_RESET, db3d::SrcName(), __LINE__, __FUNCTION__, std::format(__VA_ARGS__))
