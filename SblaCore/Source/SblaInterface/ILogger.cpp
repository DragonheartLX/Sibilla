#include "SblaInterface/ILogger.h"

namespace sbla
{
	LoggerLevel ILogger::s_Level = LoggerLevel::info;

	void ILogger::setLevel(LoggerLevel level) { s_Level = level; }

	LoggerLevel ILogger::getLevel() { return s_Level; }
} // namespace sbla