#include <cstdlib>

#include "Console.h"
#include "SblaCLI.h"
#include "SblaInterface/ILogger.h"

int main(int argc, char** argv)
{
	if (sbla::Console::init() == false) return EXIT_FAILURE;
	sbla::Console::setLoggerLevel(sbla::LoggerLevel::trace);

	if (sbla::SblaCLI::init() == false)
	{
		sbla::Console::fatal("CLI init error!");
		return EXIT_FAILURE;
	}

	if (!sbla::SblaCLI::getInstance()->run())
	{
		sbla::Console::error("CLI run error!\n Exit.");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}