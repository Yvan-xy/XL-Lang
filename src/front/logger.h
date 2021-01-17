#ifndef RJIT_FRONT_LOGGER_H
#define RJIT_FRONT_LOGGER_H

#include <string>

namespace RJIT::front {

    void setFilename(const std::string &);

    class Logger {
    private:
        static int64_t error_num, warn_num;
        int line, position;
    public:
        Logger() : line(1), position(1) {}

        Logger(int line_, int position_) :
                line(line_), position(position_) {}

        void setLogger(int line_, int pos) {
            line = line_;
            position = pos;
        }

        void LogError(const std::string &) const;

        void LogError(const std::string &, const std::string &) const;

        void LogWarning(const std::string &) const;

        void LogInfo(const std::string &) const;

        static std::size_t errorNum() { return error_num; }

        static std::size_t warningNum() { return warn_num; }
    };

    typedef std::unique_ptr<Logger> LoggerPtr;
}


#endif //RJIT_LOGGER_H
