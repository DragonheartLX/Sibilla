#include "Console.h"

#include <SblaInterface/ILogger.h>

#include <iostream>

namespace sbla
{
	Console::Console() {}

	Console::~Console() {}

	void Console::log(LoggerLevel level, const std::string& log) { std::cout << log << std::endl; }
} // namespace sbla