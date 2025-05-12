#include "Console.h"

#include <SblaCore/Macros.h>
#include <SblaInterface/ILogger.h>

#include <chrono>
#include <cstdio>
#include <format>
#include <iostream>
#include <sstream>
#include <string>

#ifdef SBL_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <conio.h>
#else
	#include <fcntl.h>
	#include <termios.h>

	#include <clocale>
#endif

/*
    
    $reset $color [HH:MM:SS][Location]: {msg} $color
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

// work unexpected why?
// static const char CLEAR_LINE[]	= "\r\x1b[2k";
static const char CLEAR_LINE[]	= "\r\x1b[0J";
static const char RESET[]		= "\x1b[0m";
static const char BOLD[]		= "\x1b[1m";

static const char WHITE_TEXT[]	= "\x1b[38;2;255;255;255m";
static const char RED_TEXT[]	= "\x1b[38;2;255;0;0";
static const char GREEN_TEXT[]	= "\x1b[38;2;0;255;0m";
static const char YELLOW_TEXT[] = "\x1b[38;2;255;255;0m";
static const char GRAY_TEXT[]	= "\x1b[38;2;192;192;192m";

static const char RED_BG[]		= "\x1b[48;2;255;0;0m";
static const char BLACK_BG[]	= "\x1b[48;2;0;0;0m";

namespace sbla
{
	Console::Console()
	{
		setLocation("CMD");
		m_IsRunning	  = true;

		m_CommandLine = std::thread([this]() {
#ifdef SBL_PLATFORM_WINDOWS
			// Windows
			HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
			DWORD mode;
			GetConsoleMode(hStdin, &mode);
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(hStdin, mode & ~(ENABLE_LINE_INPUT));

			SetConsoleCP(65001);
			SetConsoleOutputCP(65001);
#else
			// Linux/macOS
			termios oldt;
			tcgetattr(STDIN_FILENO, &oldt);
			termios newt	  = oldt;
			newt.c_lflag	 &= ~(ICANON | ECHO);
			newt.c_cc[VMIN]	  = 0;
			newt.c_cc[VTIME]  = 1;
			tcsetattr(STDIN_FILENO, TCSANOW, &newt);

			setbuf(stdout, NULL);

			std::setlocale(LC_ALL, "en_US.UTF-8");
#endif

			while (this->m_IsRunning)
			{
				char inputChar = -1;

#ifdef SBL_PLATFORM_WINDOWS
				if (_kbhit())
				{
					inputChar = _getch();
				}
#elif SBL_PLATFORM_LINUX
				struct termios newt, oldt;

				int tty = open("/dev/tty", O_RDONLY);
				tcgetattr(tty, &oldt);
				newt		  = oldt;
				newt.c_lflag &= ~(ICANON | ECHO);
				tcsetattr(tty, TCSANOW, &newt);
				read(tty, &inputChar, 1);
				tcsetattr(tty, TCSANOW, &oldt);
#endif

				std::lock_guard<std::mutex> lock(m_ConsoleMutex);

				switch (inputChar)
				{
					case -1: break;
					case '\r':
					case '\n':
						m_CommandBuffer.clear();
						m_CommandBufferUpdate();
						break;
					case 127: // DEL
					case '\b':
						if (m_CommandBuffer.size() > 0) m_CommandBuffer.pop_back();
						m_CommandBufferUpdate();
						break;
					default:
						m_CommandBuffer.push_back(inputChar);
						m_CommandBufferUpdate();
						break;
				}
			}

#ifndef SBL_PLATFORM_WINDOWS
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
		});

		m_Logger = std::thread([this]() {
			m_CommandBufferUpdate();
			while (this->m_IsRunning)
			{
				std::lock_guard<std::mutex> lock(m_ConsoleMutex);
				if (!m_LogQueue.empty())
				{
					std::cout << m_LogQueue.front();
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
			std::cout << m_LogQueue.front();
			m_LogQueue.pop();
		}
	}

	void Console::log(LoggerLevel level, const std::string& log)
	{
		if (level > getLoggerLevel()) return;

		// Get time
		std::chrono::time_point time = std::chrono::system_clock::now();
		std::time_t tt				 = std::chrono::system_clock::to_time_t(time);
		std::tm ttm;

#ifdef SBL_PLATFORM_WINDOWS
		localtime_s(&ttm, &tt);
#else
		localtime_r(&tt, &ttm);
#endif

		std::string timeFormat = std::format("[{0:0>2}:{1:0>2}:{2:0>2}]", ttm.tm_hour, ttm.tm_min, ttm.tm_sec);

		// Set color
		std::string colorFormat;
		switch (level)
		{
			case LoggerLevel::fatal: colorFormat = "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m"; break;
			case LoggerLevel::error: colorFormat = "\x1b[38;2;255;0;0m\x1b[48;2;0;0;0m"; break;
			case LoggerLevel::warning: colorFormat = "\x1b[38;2;255;255;0m\x1b[48;2;0;0;0m"; break;
			case LoggerLevel::info: colorFormat = "\x1b[38;2;0;255;0m\x1b[48;2;0;0;0m"; break;
			case LoggerLevel::debug: colorFormat = "\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m"; break;
			case LoggerLevel::trace: colorFormat = "\x1b[38;2;192;192;192m\x1b[48;2;0;0;0m"; break;
			default: return;
		}

		// Logger location
		std::string locationFormat = std::format("[{0}]: ", getLocation());

		std::istringstream logSS(log);
		for (std::string line; std::getline(logSS, line);)
		{
			std::string logFormat;
			logFormat.clear();
			logFormat += CLEAR_LINE;
			logFormat += RESET;

			logFormat  = logFormat + colorFormat + timeFormat + locationFormat + BOLD + line + RESET + "\n" + RESET + "SCLI> ";

			std::lock_guard<std::mutex> lock(m_ConsoleMutex);
			logFormat += m_CommandBuffer;
			m_LogQueue.push(logFormat);
		}
	}

	void Console::m_CommandBufferUpdate()
	{
		std::string logFormat;

		logFormat.clear();
		logFormat += CLEAR_LINE;
		logFormat  = logFormat + RESET + "SCLi> ";

		logFormat += m_CommandBuffer;
		m_LogQueue.push(logFormat);
	}
} // namespace sbla