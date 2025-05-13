#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <toml++/impl/parse_error.hpp>
#include <toml++/impl/parser.hpp>
#include <toml++/impl/std_optional.hpp>
#include <toml++/impl/table.hpp>
#include <toml++/toml.hpp>

#include "Console.h"
#include "GlobalInstance.h"
#include "SblaCore/Config.h"
#include "SblaInterface/ILogger.h"

// clang-format off
static const char templateConfig[] =
R"(# Sibilla Config Template

# Config Version
version = 1

[dev]
# fatal, error, warning, info, debug, trace
log_level = "info"

[Sibilla]

[Adapter]

[ChatBot]
)";
// clang-format on

namespace sbla
{
	GlobalInstance::GlobalInstance()
	{
		isRunning = true;

		// Load or create config
		std::filesystem::path cfgPath(std::filesystem::current_path() / "Config.toml");
		if (std::filesystem::exists(cfgPath) == false)
		{
			// Config not exist
			// dump
			Console::warn("Config not find. Create new config.");

			std::ofstream dump(cfgPath);
			if (dump.is_open() == false)
			{
				Console::fatal("Create Config error.\nPath: {0}", cfgPath.string());
				exit(EXIT_FAILURE);
			}

			dump << templateConfig;
			dump.close();
		}

		// Load Config
		toml::table cfg;
		try
		{
			cfg = toml::parse_file(cfgPath.string());
		}
		catch (const toml::parse_error& error)
		{
			Console::fatal("Error parsing file {0}:\n{1}\n(error occurred at line {2}, column {3})", *error.source().path, error.description(),
						   error.source().begin.line, error.source().begin.column);
			exit(EXIT_FAILURE);
		}

		// Config Version
		toml::optional<std::string> version = cfg["Version"].value<std::string>();

		// dev
		{
			std::string loggerLevel = cfg["dev"]["log_level"].value_or("info");

			if (loggerLevel == "fatal")
				Console::setLoggerLevel(LoggerLevel::fatal);
			else if (loggerLevel == "error")
				Console::setLoggerLevel(LoggerLevel::error);
			else if (loggerLevel == "warn")
				Console::setLoggerLevel(LoggerLevel::warn);
			else if (loggerLevel == "info")
				Console::setLoggerLevel(LoggerLevel::info);
			else if (loggerLevel == "debug")
				Console::setLoggerLevel(LoggerLevel::debug);
			else if (loggerLevel == "trace")
				Console::setLoggerLevel(LoggerLevel::trace);
			else
				Console::warn("Unknown log level: {0}, use default level.", loggerLevel);
		}

		// Sibilla
		{
		}

		// Adapter
		{
			if (cfg["Adapter"].is_table() == false)
			{
				Console::fatal("Load Adapter Config error.");
				exit(EXIT_FAILURE);
			}

			for (const auto& [titie, tables] : *cfg["Adapter"].as_table())
			{
				Config c;
				c.name = titie;

				for (const auto& [key, value] : *tables.as_table())
				{
					c.content[key.data()] = value.value_or("");
				}

				config[titie.data()] = c;
			};
		}
		// ChatBot
		{
			if (cfg["ChatBot"].is_table() == false)
			{
				Console::fatal("Load ChatBot Config error.");
				exit(EXIT_FAILURE);
			}

			for (const auto& [titie, tables] : *cfg["ChatBot"].as_table())
			{
				Config c;
				c.name = titie;

				for (const auto& [key, value] : *tables.as_table())
				{
					c.content[key.data()] = value.value_or("");
				}

				config[titie.data()] = c;
			};
		}
	}

	GlobalInstance::~GlobalInstance() {}
} // namespace sbla