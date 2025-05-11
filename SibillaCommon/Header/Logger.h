#pragma once

#include <format>
#include <mutex>
#include <queue>
#include <string>

#include "Utils/Singleton.h"

namespace scom
{
	enum class LoggerLevel
	{
		fatal = 0,
		error,
		warning,
		info,
		debug,
		trace
	};

	struct LogInfo
	{
		LoggerLevel level = LoggerLevel::info;
		std::string log	  = "";
	};

	class Logger: public Singleton<Logger>
	{
	public:
		~Logger() = default;

		void log(LoggerLevel level, const std::string& log);
		void pull(LogInfo* info);

		static void setLoggerLevel(LoggerLevel level = LoggerLevel::info);

		template <typename T>
		void log(LoggerLevel level, const std::string& format, T& value)
		{
			log(level, std::format(format, value));
		};

		template <typename T, typename... Args>
		void log(LoggerLevel level, const std::string& format, T& value, Args&&... args)
		{
			log(level, std::vformat(format, std::make_format_args(value, args...)));
		};

		template <typename T, typename... Args>
		static void fatal(const std::string& format, T& value, Args&&... args)
		{
			getInstance()->log(LoggerLevel::fatal, std::vformat(format, std::make_format_args(value, args...)));
		};

		static void fatal(const std::string& log) { getInstance()->log(LoggerLevel::fatal, log); };

		template <typename T, typename... Args>
		static void error(const std::string& format, T& value, Args&&... args)
		{
			getInstance()->log(LoggerLevel::error, std::vformat(format, std::make_format_args(value, args...)));
		};

		static void error(const std::string& log) { getInstance()->log(LoggerLevel::error, log); };

		template <typename T, typename... Args>
		static void warning(const std::string& format, T& value, Args&&... args)
		{
			getInstance()->log(LoggerLevel::warning, std::vformat(format, std::make_format_args(value, args...)));
		};

		static void warning(const std::string& log) { getInstance()->log(LoggerLevel::warning, log); };

		template <typename T, typename... Args>
		static void info(const std::string& format, T& value, Args&&... args)
		{
			getInstance()->log(LoggerLevel::info, std::vformat(format, std::make_format_args(value, args...)));
		};

		static void info(const std::string& log) { getInstance()->log(LoggerLevel::info, log); };

		template <typename T, typename... Args>
		static void debug(const std::string& format, T& value, Args&&... args)
		{
			getInstance()->log(LoggerLevel::debug, std::vformat(format, std::make_format_args(value, args...)));
		};

		static void debug(const std::string& log) { getInstance()->log(LoggerLevel::debug, log); };

		template <typename T, typename... Args>
		static void trace(const std::string& format, T& value, Args&&... args)
		{
			getInstance()->log(LoggerLevel::trace, std::vformat(format, std::make_format_args(value, args...)));
		};

		static void trace(const std::string& log) { getInstance()->log(LoggerLevel::trace, log); };

	private:
		static LoggerLevel s_Level;

		std::mutex m_LoggerMutex;
		std::queue<LogInfo> m_LoggerQueue;

		friend class Singleton<Logger>;
		Logger() = default;
	};
} // namespace scom