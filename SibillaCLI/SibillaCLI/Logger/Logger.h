#pragma once
#include "Utils/Singleton.h"

#include <format>
#include <queue>

namespace scli
{
    enum LoggerLevel
    {
        fatal = 0,
        error,
        warning,
        info,
        debug,
        trace
    };

    class Logger: public Singleton<Logger>
    {
    public:
        ~Logger();

        void log(LoggerLevel level, std::string log);
        void logCommand();
        
        template<typename T>
        void log(LoggerLevel level, std::string format, T value)
        {
            log(level, std::format(format, value));
        };

        template<typename T, typename... Args>
        void log(LoggerLevel level, std::string format, T value, Args... args)
        {
            log(level, std::vformat(format, std::make_format_args(value, args...)));
        };

        template<typename T, typename... Args>
        static void fatal(std::string format, T value, Args... args)
        {
            Logger::getInstance()->log(LoggerLevel::fatal, std::vformat(format, std::make_format_args(value, args...)));
        };

        template<typename T, typename... Args>
        static void error(std::string format, T value, Args... args)
        {
            Logger::getInstance()->log(LoggerLevel::error, std::vformat(format, std::make_format_args(value, args...)));
        };

        template<typename T, typename... Args>
        static void warning(std::string format, T value, Args... args)
        {
            Logger::getInstance()->log(LoggerLevel::warning, std::vformat(format, std::make_format_args(value, args...)));
        };

        template<typename T, typename... Args>
        static void info(std::string format, T value, Args... args)
        {
            Logger::getInstance()->log(LoggerLevel::info, std::vformat(format, std::make_format_args(value, args...)));
        };

        template<typename T, typename... Args>
        static void debug(std::string format, T value, Args... args)
        {
            Logger::getInstance()->log(LoggerLevel::debug, std::vformat(format, std::make_format_args(value, args...)));
        };

        template<typename T, typename... Args>
        static void trace(std::string format, T value, Args... args)
        {
            Logger::getInstance()->log(LoggerLevel::trace, std::vformat(format, std::make_format_args(value, args...)));
        };

        static void fatal(std::string log)
        {
            Logger::getInstance()->log(LoggerLevel::fatal, log);
        };

        static void error(std::string log)
        {
            Logger::getInstance()->log(LoggerLevel::error, log);
        };

        static void warning(std::string log)
        {
            Logger::getInstance()->log(LoggerLevel::warning, log);
        };

        static void info(std::string log)
        {
            Logger::getInstance()->log(LoggerLevel::info, log);
        };

        static void debug(std::string log)
        {
            Logger::getInstance()->log(LoggerLevel::debug, log);
        };

        static void trace(std::string log)
        {
            Logger::getInstance()->log(LoggerLevel::trace, log);
        };

        void run(bool isRunning);
        static void setLoggerLevel(LoggerLevel level);
    private:
        static LoggerLevel s_Level;

        std::queue<std::string> m_LogQueue;

        friend class Singleton<Logger>;
        Logger();
    };
}