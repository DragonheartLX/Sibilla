#include <cstdlib>

#include "Console.h"

int main(int argc, char** argv)
{
	sbla::Console::info("{0}, {1}", 3.13, 333);
	return EXIT_SUCCESS;
}