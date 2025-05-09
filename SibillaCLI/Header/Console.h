#pragma once
#include "Utils/Singleton.h"

#include <mutex>
#include <thread>
#include <atomic>

#include <string>
#include <fmt/format.h>
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

    class Console:public Singleton<Console>
    {
    public:
        ~Console();
        
        void log(LoggerLevel level, const std::string& log);
        static void setLoggerLevel(LoggerLevel level = LoggerLevel::info);

        template<typename T>
        void log(LoggerLevel level, const std::string& format, T value)
        {
            log(level, fmt::format(format, value));
        };

        template<typename T, typename... Args>
        void log(LoggerLevel level, const std::string& format, T value, Args&&... args)
        {
            log(level, fmt::vformat(format, fmt::make_format_args(value, args...)));
        };

        template<typename T, typename... Args>
        static void fatal(const std::string& format, T value, Args&&... args)
        {
            getInstance()->log(LoggerLevel::fatal, fmt::vformat(format, fmt::make_format_args(value, args...)));
        };

        static void fatal(const std::string& log)
        {
            getInstance()->log(LoggerLevel::fatal, log);
        };

        template<typename T, typename... Args>
        static void error(const std::string& format, T value, Args&&... args)
        {
            getInstance()->log(LoggerLevel::error, fmt::vformat(format, fmt::make_format_args(value, args...)));
        };

        static void error(const std::string& log)
        {
            getInstance()->log(LoggerLevel::error, log);
        };

        template<typename T, typename... Args>
        static void warning(const std::string& format, T value, Args&&... args)
        {
            getInstance()->log(LoggerLevel::warning, fmt::vformat(format, fmt::make_format_args(value, args...)));
        };

        static void warning(const std::string& log)
        {
            getInstance()->log(LoggerLevel::warning, log);
        };
        
        template<typename T, typename... Args>
        static void info(const std::string& format, T value, Args&&... args)
        {
            getInstance()->log(LoggerLevel::info, fmt::vformat(format, fmt::make_format_args(value, args...)));
        };

        static void info(const std::string& log)
        {
            getInstance()->log(LoggerLevel::info, log);
        };

        template<typename T, typename... Args>
        static void debug(const std::string& format, T value, Args&&... args)
        {
            getInstance()->log(LoggerLevel::debug, fmt::vformat(format, fmt::make_format_args(value, args...)));
        };

        static void debug(const std::string& log)
        {
            getInstance()->log(LoggerLevel::debug, log);
        };

        template<typename T, typename... Args>
        static void trace(const std::string& format, T value, Args&&... args)
        {
            getInstance()->log(LoggerLevel::trace, fmt::vformat(format, fmt::make_format_args(value, args...)));
        };

        static void trace(const std::string& log)
        {
            getInstance()->log(LoggerLevel::trace, log);
        };
    private:
    
        void m_LogCommand();

        std::mutex m_ConsoleMutex;
        std::thread m_CommandLine;
        std::thread m_Logger;
        
        std::atomic<bool> m_IsRunning;
        static LoggerLevel s_Level;
        std::string m_CommandBuffer;
        std::queue<std::string> m_LogQueue;

        friend class Singleton<Console>;
        Console();
    };
}