#pragma once
#include <SblaCore/Utils/Singleton.h>
#include <SblaInterface/ILogger.h>

#include <atomic>
#include <queue>
#include <string>

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
			getInstance()->log(LoggerLevel::fatal, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void error(std::format_string<Args...> fmt, Args&&... args)
		{
			getInstance()->log(LoggerLevel::error, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void warning(std::format_string<Args...> fmt, Args&&... args)
		{
			getInstance()->log(LoggerLevel::warning, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void info(std::format_string<Args...> fmt, Args&&... args)
		{
			getInstance()->log(LoggerLevel::info, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void debug(std::format_string<Args...> fmt, Args&&... args)
		{
			getInstance()->log(LoggerLevel::debug, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void trace(std::format_string<Args...> fmt, Args&&... args)
		{
			getInstance()->log(LoggerLevel::trace, std::format(fmt, std::forward<Args>(args)...));
		}

	private:
		std::atomic<bool> m_IsRunning;

		std::mutex m_ConsoleMutex;
		std::string m_CommandBuffer;
		std::queue<std::string> m_LogQueue;

		std::thread m_CommandLine;
		std::thread m_Logger;

		void m_CommandBufferUpdate();

		friend class Singleton<Console>;
		Console();
	};
} // namespace sbla