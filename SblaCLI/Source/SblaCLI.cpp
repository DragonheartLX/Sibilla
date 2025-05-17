#include "SblaCLI.h"

#include <SblaCore/Config.h>
#include <SblaInterface/Plugin.h>

#include <filesystem>

#include "Console.h"
#include "GlobalInstance.h"
#include "Loader.h"

namespace sbla
{
	SblaCLI::SblaCLI() {}

	SblaCLI::~SblaCLI()
	{
		for (auto& [key, value] : m_Ada) delete value;
		for (auto& [key, value] : m_CB) delete value;
	}

	bool SblaCLI::run()
	{
		std::filesystem::path currentPath = std::filesystem::current_path();
		std::filesystem::path pluginsPath = currentPath / "Plugins";
		std::filesystem::path cfgPath	  = currentPath / "Config.cfg";

		// config handle
		if (m_IniParser.LoadFromFile(cfgPath.string().c_str()) != 0) return false;

		// // Load ChatBot and Adapter
		if (!std::filesystem::exists(pluginsPath) || !std::filesystem::is_directory(pluginsPath))
			Console::warn("No Plugin find,");
		else
		{
			for (const auto& entry : std::filesystem::directory_iterator(pluginsPath))
			{
				Loader* lib = new Loader(pluginsPath.string(), entry.path().filename().string());

				if (!lib->load())
				{
					delete lib;
					continue;
				};

				PluginInfo* info = lib->getInfo();
			}
		}

		while (GlobalInstance::Instance()->isRunning);
		return true;
	}
} // namespace sbla