#include "Logger.h"

#include <mutex>

namespace scom
{
	LoggerLevel Logger::s_Level = LoggerLevel::info;

	void Logger::log(LoggerLevel level, const std::string& log)
	{
		if (level > s_Level) return;

		std::lock_guard<std::mutex> lock(m_LoggerMutex);
		m_LoggerQueue.push({level, log});
	}

	void Logger::pull(LogInfo* info)
	{
		std::lock_guard<std::mutex> lock(m_LoggerMutex);
		*info = m_LoggerQueue.front();
		m_LoggerQueue.pop();
	}
} // namespace scom