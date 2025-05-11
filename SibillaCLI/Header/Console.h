#pragma once
#include <Logger.h>
#include <SibillaCommon.h>
#include <Utils/Singleton.h>

#include <string>
#include <utility>

namespace scli
{
	class Console: public scom::Singleton<Console>
	{
	public:
		~Console();

		void log(std::string locate, scom::LogInfo* info);

	private:
		std::mutex m_ConsoleMutex;
		std::thread m_CommandLine;
		std::thread m_Logger;

		std::queue<std::pair<std::string, scom::LogInfo>> m_LogQueue;

		friend class scom::Singleton<Console>;
		Console();
	};
} // namespace scli