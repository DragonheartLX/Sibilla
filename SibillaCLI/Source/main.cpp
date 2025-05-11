#include <Logger.h>

#include "Console.h"
#include "SibillaCLI.h"

int main(int argc, char** argv)
{
	if (!scli::Console::init()) return EXIT_FAILURE;

	scom::Logger::bind([](scom::LoggerLevel level, const std::string& log, const std::string& location) {
		scli::Console::getInstance()->log(level, log, location);
	});

	scom::Logger::setLoggerLocation("CMD");

	scli::SibillaCLI* cli = new scli::SibillaCLI;

	if (!cli->run())
	{
		scom::Logger::error("Exit error.");

		delete cli;
		return EXIT_FAILURE;
	}

	scom::Logger::error("Exit.");

	delete cli;
	return EXIT_SUCCESS;
}