#include "Logger.h"

Logger* Logger::instance = 0;

Logger::Logger()
{
    m_enabled = true;
}

Logger::~Logger()
{
    delete instance;
}

void Logger::log(LogLevel level, std::string text)
{
    fprintf(stdout, "%s: %s\n", prefix_from_log_level(level), text.c_str());
}

void Logger::log(std::string text)
{
    log(LogLevel::INFO, text);
}

Logger* Logger::the()
{
    if (instance == 0) {
        instance = new Logger();
    }

    return instance;
}

char* Logger::prefix_from_log_level(LogLevel level) {
    if (level == LogLevel::INFO) return (char*) "[INFO]";
    else if (level == LogLevel::WARNING) return (char*) "[WARNING]";
    else if (level == LogLevel::FATAL) return (char*) "[ERROR]";

    else return (char*) "[OTHER]";
}
