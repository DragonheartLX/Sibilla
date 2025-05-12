#include "SblaInterface/ILogger.h"

namespace sbla
{
	LoggerLevel ILogger::s_Level	= LoggerLevel::info;
	std::string ILogger::s_location = "NULL";

	void ILogger::setLoggerLevel(LoggerLevel level) { s_Level = level; }

	LoggerLevel ILogger::getLoggerLevel() { return s_Level; }

	void ILogger::setLocation(const std::string& location) { s_location = location; }

	const std::string& ILogger::getLocation() { return s_location; }
} // namespace sbla