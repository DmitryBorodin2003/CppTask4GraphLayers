#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    explicit Logger(std::string filename);

    void logMessage(const std::string &message, LogLevel level);

private:
    std::string logFilename;

    std::string getLevelString(LogLevel level);
};

#endif // LOGGER_H
