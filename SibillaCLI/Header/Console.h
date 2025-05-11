#pragma once
#include <Logger.h>
#include <SibillaCommon.h>
#include <Utils/Singleton.h>

#include <atomic>
#include <string>

// #include <string>
// #include <utility>

namespace scli
{
	class Console: public scom::Singleton<Console>
	{
	public:
		~Console();

		void log(scom::LoggerLevel level, const std::string& log, const std::string& location);

	private:
		void m_CommandBufferUpdate();

		std::atomic<bool> m_IsRunning;

		std::mutex m_ConsoleMutex;
		std::queue<std::string> m_LogQueue;

		std::thread m_CommandLine;
		std::thread m_Logger;

		std::string m_CommandBuffer;

		friend class scom::Singleton<Console>;
		Console();
	};
} // namespace scli