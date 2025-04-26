#include "Logger/Logger.h"
#include "Command/Command.h"
#include "Utils/GlobalInstance.h"

#include <ctime>
#include <chrono>
#include <BS_thread_pool.hpp>

BS::synced_stream syncOut;

namespace scli
{
    /*
    
        $reset $color [HH:MM:SS][Level]: {msg} $color
        $reset CMD Output
        $reset SCLI > 
    
    */

    /*
    
        ansi escape code

        \0x1b + "[" + <zero or more numbers, separated by ";"> + <a letter>

        \x1b[2k             Clear line
        \x1b[0m             Reset color
        \x1b[1m             Bold
        \x1b[3m             Italic
        \x1b[4m             Underline
        \x1b[38;2;r;g;bm    Set text colour to an RGB value
        \x1b[48;2;r;g;bm    Set background colour to an RGB value

        \x1b[?25h           Show cursor
        \x1b[?25l           Hide cursor

    */

    /*
        Text            Background      Level 

        255, 255, 255   255,   0,   0   Fatal       \x1b[0m\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m\x1b[1m{Msg}\x1b[0m
        255,   0,   0     0,   0,   0   Error       \x1b[0m\x1b[38;2;255;0;0m\x1b[48;2;0;0;0m\x1b[1m{Msg}\x1b[0m
        255, 255,   0     0,   0,   0   Waring      \x1b[0m\x1b[38;2;255;255;0m\x1b[48;2;0;0;0m\x1b[1m{Msg}\x1b[0m
          0, 255,   0     0,   0,   0   Info        \x1b[0m\x1b[38;2;0;255;0m\x1b[48;2;0;0;0m\x1b[1m{Msg}\x1b[0m
        255, 255, 255     0,   0,   0   Debug       \x1b[0m\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m\x1b[1m{Msg}\x1b[0m
        192, 192, 192     0,   0,   0   Trace       \x1b[0m\x1b[38;2;192;192;192m\x1b[48;2;0;0;0m\x1b[1m{Msg}\x1b[0m

    */

    LoggerLevel Logger::s_Level = LoggerLevel::info;

    Logger::Logger()
    {

    }

    Logger::~Logger()
    {
        std::lock_guard<std::mutex> lock(GlobalInstance::getInstance()->consoleMutex);
        while (!m_LogQueue.empty())
        {
            syncOut.print(m_LogQueue.front());
            m_LogQueue.pop();
        }
    }

    void Logger::log(LoggerLevel level, std::string log)
    {
        if (level > s_Level)
            return;

        std::string logStr;

        logStr.clear();
        logStr += "\r\x1b[2K";
        logStr += "\x1b[0m";

        // Get time
        std::chrono::time_point time = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(time);
        std::tm ttm;
        localtime_s(&ttm, &tt);

        std::string timeStr = fmt::format("[{0:0>2}:{1:0>2}:{2:0>2}]", ttm.tm_hour, ttm.tm_min, ttm.tm_sec);

        switch (level)
        {
        case LoggerLevel::fatal:        logStr = logStr + "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m" + timeStr + "[fatal]: ";     break;
        case LoggerLevel::error:        logStr = logStr + "\x1b[38;2;255;0;0m\x1b[48;2;0;0;0m"       + timeStr + "[error]: ";     break;
        case LoggerLevel::warning:      logStr = logStr + "\x1b[38;2;255;255;0m\x1b[48;2;0;0;0m"     + timeStr + "[warning]: ";   break;
        case LoggerLevel::info:         logStr = logStr + "\x1b[38;2;0;255;0m\x1b[48;2;0;0;0m"       + timeStr + "[info]: ";      break;
        case LoggerLevel::debug:        logStr = logStr + "\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m"   + timeStr + "[debug]: ";     break;
        case LoggerLevel::trace:        logStr = logStr + "\x1b[38;2;192;192;192m\x1b[48;2;0;0;0m"   + timeStr + "[trace]: ";     break;
        default: return;
        }

        logStr += "\x1b[1m";
        logStr += log;
        logStr += "\x1b[0m";
        logStr += "\n";

        logStr += "\x1b[0mSCLi> ";

        std::lock_guard<std::mutex> lock(GlobalInstance::getInstance()->consoleMutex);
        logStr += Command::getInstance()->inputCommand;
        m_LogQueue.push(logStr);
    }

    void Logger::logCommand()
    {
        std::string logStr;

        logStr.clear();
        logStr += "\r\x1b[2K";
        logStr += "\x1b[0mSCLi> ";

        logStr += Command::getInstance()->inputCommand;
        m_LogQueue.push(logStr);
    }

    void Logger::run(bool isRunning)
    {
        while (isRunning)
        {
            std::lock_guard<std::mutex> lock(GlobalInstance::getInstance()->consoleMutex);
            if (!m_LogQueue.empty())
            {
                syncOut.print(m_LogQueue.front());
                m_LogQueue.pop();
            }
        }
    }

    void Logger::setLoggerLevel(LoggerLevel level)
    {
        s_Level = level;
    }
}