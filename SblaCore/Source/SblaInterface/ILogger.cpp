#include "SblaInterface/ILogger.h"

namespace sbla
{
	LoggerLevel ILogger::s_Level = LoggerLevel::info;

	void ILogger::setLoggerLevel(LoggerLevel level) { s_Level = level; }

	LoggerLevel ILogger::getLoggerLevel() { return s_Level; }
} // namespace sbla