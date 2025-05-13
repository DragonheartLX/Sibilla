#pragma once

#include <format>
#include <string>
#include <utility>

namespace sbla
{
	enum class LoggerLevel
	{
		fatal = 0,
		error,
		warn,
		info,
		debug,
		trace
	};

	class ILogger
	{
	public:
		ILogger()													= default;
		~ILogger()													= default;

		virtual void log(LoggerLevel level, const std::string& log) = 0;

		static void setLoggerLevel(LoggerLevel level);
		static LoggerLevel getLoggerLevel();

		// template <typename... Args>
		// void log_format(LoggerLevel level, std::format_string<Args...> fmt, Args&&... args)
		// {
		// 	this->log(level, std::format(fmt, std::forward<Args>(args)...));
		// }

		template <typename... Args>
		void fatal(std::format_string<Args...> fmt, Args&&... args)
		{
			this->log(LoggerLevel::fatal, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void error(std::format_string<Args...> fmt, Args&&... args)
		{
			this->log(LoggerLevel::error, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void warn(std::format_string<Args...> fmt, Args&&... args)
		{
			this->log(LoggerLevel::warn, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void info(std::format_string<Args...> fmt, Args&&... args)
		{
			this->log(LoggerLevel::info, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void debug(std::format_string<Args...> fmt, Args&&... args)
		{
			this->log(LoggerLevel::debug, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		void trace(std::format_string<Args...> fmt, Args&&... args)
		{
			this->log(LoggerLevel::trace, std::format(fmt, std::forward<Args>(args)...));
		}

	private:
		static LoggerLevel s_Level;
	};
} // namespace sbla