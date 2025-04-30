#include "Console.h"
#include "Utils/Config.h"

#include <ctime>
#include <chrono>
#include <BS_thread_pool.hpp>

#ifdef _WIN32
    #include <Windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <fcntl.h>
    #include <clocale>
#endif

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

    LoggerLevel Console::s_Level = LoggerLevel::info;

    Console::Console()
    {
        m_IsRunning = true;

        m_CommandLine = std::thread([this]()
        {
            #ifdef _WIN32
                // Windows
                HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
                DWORD mode;
                GetConsoleMode(hStdin, &mode);
                SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT));

                SetConsoleOutputCP(65001);
            #else
                // Linux/macOS
                termios oldt;
                tcgetattr(STDIN_FILENO, &oldt);
                termios newt = oldt;
                newt.c_lflag &= ~(ICANON | ECHO);
                newt.c_cc[VMIN] = 0;
                newt.c_cc[VTIME] = 1;
                tcsetattr(STDIN_FILENO, TCSANOW, &newt);

                setbuf(stdout, NULL);

                std::setlocale(LC_ALL, "en_US.UTF-8")
            #endif

            while (this->m_IsRunning)
            {
                char inputChar = -1;

                #if _WIN32
                    if (_kbhit())
                    {
                        inputChar = _getche();
                    }
                #elif __linux__
                    struct termios newt, oldt;
                    
                    int tty = open("/dev/tty", O_RDONLY);
                    tcgetattr(tty, &oldt);
                    newt = oldt;
                    newt.c_lflag &= ~( ICANON | ECHO );
                    tcsetattr(tty, TCSANOW, &newt);
                    read(tty, &inputChar, 1);
                    tcsetattr(tty, TCSANOW, &oldt);
                #endif

                std::lock_guard<std::mutex> lock(m_ConsoleMutex);

                switch (inputChar)
                {
                case -1:
                    break;
                case '\r':
                case '\n':
                    m_CommandBuffer.clear();
                    m_LogCommand();
                    break;
                case 127: // DEL
                case '\b':
                    if (m_CommandBuffer.size() > 0)
                        m_CommandBuffer.pop_back();
                        m_LogCommand();
                    break;
                default:
                    m_CommandBuffer.push_back(inputChar);
                    m_LogCommand();
                    break;
                }
            }

            #ifndef _WIN32
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            #endif
        });

        m_Logger = std::thread([this]()
        {
            while (this->m_IsRunning)
        {
            std::lock_guard<std::mutex> lock(m_ConsoleMutex);
            if (!m_LogQueue.empty())
            {
                syncOut.print(m_LogQueue.front());
                m_LogQueue.pop();
            }
        }
        });
    }

    Console::~Console()
    {
        m_IsRunning = false;

        m_CommandLine.join();
        m_Logger.join();
    
        std::lock_guard<std::mutex> lock(m_ConsoleMutex);
        while (!m_LogQueue.empty())
        {
            syncOut.print(m_LogQueue.front());
            m_LogQueue.pop();
        }
    }

    void Console::log(LoggerLevel level, std::string log)
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

        #ifdef _WIN32
            localtime_s(&ttm, &tt);
        #else
            localtime_r(&tt, &ttm);
        #endif

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

        std::lock_guard<std::mutex> lock(m_ConsoleMutex);
        logStr += m_CommandBuffer;
        m_LogQueue.push(logStr);
    }

    void Console::m_LogCommand()
    {
        std::string logStr;

        logStr.clear();
        logStr += "\r\x1b[2K";
        logStr += "\x1b[0mSCLi> ";

        logStr += m_CommandBuffer;
        m_LogQueue.push(logStr);
    }

    void Console::setLoggerLevel()
    {
        s_Level = (LoggerLevel)Config::getInstance()->data["dev"]["log_level"].value_or(3);
    }
}