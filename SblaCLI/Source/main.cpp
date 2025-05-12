#include <chrono>
#include <cstdlib>
#include <thread>

#include "Console.h"
#include "SblaInterface/ILogger.h"

int main(int argc, char** argv)
{
	if (sbla::Console::init() == false) return EXIT_FAILURE;
	sbla::Console::setLoggerLevel(sbla::LoggerLevel::trace);

	sbla::Console::info("init.");

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		sbla::Console::info("Tick.");
	};
	return EXIT_SUCCESS;
}