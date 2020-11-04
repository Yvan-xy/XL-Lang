#include "logger.h"
#include "define/color.h"
#include <iostream>

namespace RJIT::front {

    std::string_view filename;

    int64_t  Logger::error_num = 0, Logger::warn_num = 0;
    void setFilename(const std::string &file) {
        filename = file;
    }

    void Logger::LogError(const std::string &info) const {
        auto red = Color::Modifier(Color::Code::FG_RED);
        auto def = Color::Modifier(Color::Code::FG_DEFAULT);
        std::cerr << filename << ":"
                  << line << ":"
                  << position << ": "
                  << red << "error: " << def
                  << info << std::endl;
        error_num++;
    }

    void Logger::LogWarning(const std::string &info) const {
        auto pink = Color::Modifier(Color::Code::FG_PINK);
        auto def = Color::Modifier(Color::Code::FG_DEFAULT);
        std::cerr << filename << ":"
                  << line << ":"
                  << position << ": "
                  << pink << "warning: " << def
                  << info << std::endl;
        warn_num++;
    }

    void Logger::LogInfo(const std::string &info) const {
        auto yel = Color::Modifier(Color::Code::FG_YELLOW);
        auto def = Color::Modifier(Color::Code::FG_DEFAULT);
        std::cerr << filename << ":"
                  << line << ":"
                  << position << ": "
                  << yel << "info: " << def
                  << info << std::endl;
    }
}