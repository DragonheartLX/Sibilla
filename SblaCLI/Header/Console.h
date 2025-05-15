#pragma once
#include <SblaCore/Utils/Singleton.h>
#include <SblaInterface/ILogger.h>

#include <queue>
#include <string>
#include <thread>

namespace sbla
{
	class Console: public ILogger, public Singleton<Console>
	{
	public:
		~Console();
		virtual void log(LoggerLevel level, const std::string& log) override;

		template <typename... Args>
		static void fatal(std::format_string<Args...> fmt, Args&&... args)
		{
			Instance()->log(LoggerLevel::fatal, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void error(std::format_string<Args...> fmt, Args&&... args)
		{
			Instance()->log(LoggerLevel::error, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void warn(std::format_string<Args...> fmt, Args&&... args)
		{
			Instance()->log(LoggerLevel::warn, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void info(std::format_string<Args...> fmt, Args&&... args)
		{
			Instance()->log(LoggerLevel::info, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void debug(std::format_string<Args...> fmt, Args&&... args)
		{
			Instance()->log(LoggerLevel::debug, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void trace(std::format_string<Args...> fmt, Args&&... args)
		{
			Instance()->log(LoggerLevel::trace, std::format(fmt, std::forward<Args>(args)...));
		}

	private:
		void m_CommandLineFunc();
		void m_LoggerFunc();
		void m_CommandFunc();
		void m_CommandBufferUpdate();

		std::mutex m_ConsoleMutex;
		std::string m_CommandBuffer;
		std::queue<std::string> m_LogQueue;

		std::mutex m_CommandMutex;
		std::queue<std::string> m_CommandQueue;

		std::thread m_CommandLine;
		std::thread m_Logger;
		std::thread m_Command;

		friend class Singleton<Console>;
		Console();
	};
} // namespace sbla