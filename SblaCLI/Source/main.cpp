#include <cstdlib>

#include "Console.h"
#include "GlobalInstance.h"
#include "SblaCLI.h"

int main(int argc, char** argv)
{
	if (sbla::Console::init() == false) return EXIT_FAILURE;

	if (sbla::GlobalInstance::init() == false)
	{
		sbla::Console::fatal("Instance init error!");
		return EXIT_FAILURE;
	}

	if (sbla::SblaCLI::init() == false)
	{
		sbla::Console::fatal("CLI init error!");
		return EXIT_FAILURE;
	}

	if (!sbla::SblaCLI::Instance()->run())
	{
		sbla::Console::error("CLI run error!\n Exit.");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}