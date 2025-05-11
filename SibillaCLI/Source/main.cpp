#include "SibillaCLI.h"

int main(int argc, char** argv)
{
	scli::SibillaCLI* cli = new scli::SibillaCLI;

	if (!cli->run())
	{
		delete cli;
		return EXIT_FAILURE;
	}

	delete cli;
	return EXIT_SUCCESS;
}