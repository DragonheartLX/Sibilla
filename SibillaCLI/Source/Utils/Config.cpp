#include "Utils/Config.h"
#include "Console.h"

#include <filesystem>

namespace scli
{
    Config::Config()
    {
        std::filesystem::path cfgPath = std::filesystem::current_path() / "Config.toml";

        if (std::filesystem::exists(cfgPath))
            Console::info("Loading config.");
        else
        {
            Console::info("Can not find config file. Generate default config.");
            
            // Generate default config

        };

        try
        {
            data = toml::parse_file(cfgPath.string());
        } 
        catch (const toml::parse_error& err)
        {
            Console::error("Load config error.");
            Console::error("Path: {0}", *(err.source().path));
            Console::error("{0}", err.description());
            Console::error("Line: {0}, Column: {1}", err.source().begin.line, err.source().begin.column);
        }
    }

    Config::~Config()
    {
        
    }
}