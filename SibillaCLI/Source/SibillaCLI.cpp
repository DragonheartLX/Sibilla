#include "SibillaCLI.h"

#include <Logger.h>

#include <chrono>
#include <thread>

namespace scli
{
	SibillaCLI::SibillaCLI() { scom::Logger::info("SibillaCLI init.\nConsole logger test."); }

	SibillaCLI::~SibillaCLI() { m_IsRunning = false; }

	bool SibillaCLI::run()
	{
		m_IsRunning = true;
		while (true)
		{
			scom::Logger::info("SibillaCLI loop.\nConsole logger test.");
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
} // namespace scli