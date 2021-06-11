#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <string>

enum LogLevel
{
    INFO,
    WARNING,
    FATAL
};

class Logger
{
public:

    void log(LogLevel level, std::string text);
    void log(std::string text);
    bool is_enabled() { return m_enabled; };
    void set_enabled(bool enabled) { m_enabled = enabled; };

    static Logger* the();

private:
    static Logger* instance;
    Logger();
    ~Logger();

    bool m_enabled;

    char* prefix_from_log_level(LogLevel level); 
};

#endif
