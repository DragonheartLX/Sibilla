#include "Logger.h"

#include <string>

namespace scom
{
	LoggerLevel Logger::s_Level			 = LoggerLevel::info;
	LoggerCallBack Logger::s_LogCallBack = nullptr;
	std::string Logger::s_Location		 = "NULL";

	void Logger::log(LoggerLevel level, const std::string& log)
	{
		if (s_Level < level) return;
		s_LogCallBack(level, log, s_Location);
	}

	void Logger::bind(LoggerCallBack cb) { s_LogCallBack = cb; }

	void Logger::setLoggerLevel(LoggerLevel level) { s_Level = level; }
	void Logger::setLoggerLocation(const std::string& location) { s_Location = location; }
} // namespace scom