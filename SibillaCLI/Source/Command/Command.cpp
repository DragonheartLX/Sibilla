#include "Command/Command.h"
#include "Utils/GlobalInstance.h"
#include "Logger/Logger.h"

#ifdef _WIN32
    #include <Windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

namespace scli
{
    Command::Command()
    {

    }

    Command::~Command()
    {

    }

    void Command::run(bool isRunning)
    {
        #ifdef _WIN32
            // Windows
            HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
            DWORD mode;
            GetConsoleMode(hStdin, &mode);
            SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT));
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
        #endif

        while (isRunning)
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

            std::lock_guard<std::mutex> lock(GlobalInstance::getInstance()->consoleMutex);

            switch (inputChar)
            {
            case -1:
                break;
            case '\r':
            case '\n':
                inputCommand.clear();
                scli::Logger::getInstance()->logCommand();
                break;
            case 127: // DEL
            case '\b':
                if (inputCommand.size() > 0)
                    inputCommand.pop_back();
                scli::Logger::getInstance()->logCommand();
                break;
            default:
                inputCommand.push_back(inputChar);
                scli::Logger::getInstance()->logCommand();
                break;
            }
        }

        #ifndef _WIN32
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        #endif
    }
}