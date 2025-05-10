#include <cstdlib>
#include <toml++/toml.hpp>

#include "SibillaCLI.h"
#include "Console.h"

int main(int argc, char** argv)
{
    if (!scli::Console::init()) return EXIT_FAILURE;

    toml::table cfg = toml::parse_file("./Config.toml");
    scli::Console::setLoggerLevel(
        (scli::LoggerLevel)cfg["dev"]["log_level"].value_or(3));

    scli::Console::debug("Console init");

    scli::SibillaCLI* cli = new scli::SibillaCLI();

    if (!cli->run())
    {
        delete cli;
        return EXIT_FAILURE;
    }

    delete cli;
    return EXIT_SUCCESS;
}