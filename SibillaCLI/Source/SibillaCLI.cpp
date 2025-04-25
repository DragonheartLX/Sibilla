#include <iostream>
#include <string>

#if _WIN32
    #include <conio.h>
#elif __linux__
    #include <termio.h>
    #define TTY_PATH            "/dev/tty"
    #define STTY_US             "stty raw -echo -F"
    #define STTY_DEF            "stty -raw echo -F"
#endif

#include <curl/curl.h>
#include <csignal>

int getInputStr(std::string &inputStr)
{
    int flag = 0;
    char inputChar = -1;

    #if _WIN32
        if (_kbhit())
        {
            inputChar = _getche();
        }
    #elif __linux__
        fd_set rfds;
        struct timeval tv;
        system(STTY_US TTY_PATH);
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 10;
        if (select(1, &rfds, NULL, NULL, &tv) > 0)
        {
            inputChar = getchar();
        }
    #endif

    switch (inputChar)
    {
    case -1:
        break;
    case '\r':
    case '\n':
        flag = 2;
        break;
    case 127: // DEL
    case '\b':
        if (inputStr.size() > 0)
            inputStr.pop_back();
        flag = 1;
        break;
    default:
        inputStr.push_back(inputChar);
        flag = 1;
        break;
    }

    return flag;
}

void signalHandler(int signum)
{

    /*
    SIGABRT	程序的异常终止，如调用 abort。
    SIGFPE	错误的算术运算，比如除以零或导致溢出的操作。
    SIGILL	检测非法指令。
    SIGINT	程序终止(interrupt)信号。
    SIGSEGV	非法访问内存。
    SIGTERM	发送到程序的终止请求。
    */

    exit(signum);
}

int main(int argc, char **argv)
{

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    // spdlog::set_pattern("%^[%T] [%l]: %v%$");
    // spdlog::set_level(spdlog::level::level_enum::trace);

    // spdlog::debug("Init libcurl");
    // spdlog::debug("libcurl verion: {0}", curl_version());

    curl_global_init(CURL_GLOBAL_ALL);

    bool running = true;
    int tmp = 0;

    std::string input;
    std::cout << "\r\x1b[2Kinput: " << input;

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
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    #endif

    while (running)
    {
        int flag = getInputStr(input);
        if(flag != 0)
        {
            std::cout << "\r\x1b[2K" << "input: " << input;
        }

        if (flag == 2)
        {
            std::cout << "\r\x1b[2K" << "Get cmd: " << input << "\n";
            input.clear();
            std::cout << "input: " << input;
        }

        tmp = (tmp + 1) % 100000;
        if (tmp == 99999) std::cout << "\r\x1b[2KHB\n" << "input: " << input;
    }

    #ifndef _WIN32
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #endif

    curl_global_cleanup();

    return 0;
}