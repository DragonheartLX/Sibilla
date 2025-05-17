#include "SblaCLI.h"

#include <SblaCore/Config.h>
#include <SblaInterface/Plugin.h>

#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <utility>

#include "Console.h"
#include "GlobalInstance.h"
#include "Loader.h"

// clang-format off
static const char default_config[] =
R"(# Sibilla Config Template

# Config Version
version = 1

[dev]
# fatal, error, warning, info, debug, trace
log_level = "info"

[Sibilla]
)";
// clang-format on

namespace sbla
{
	SblaCLI::SblaCLI() {}

	SblaCLI::~SblaCLI()
	{
		for (auto& val : m_Inst)
		{
			val.chatbot->onDisable();
			val.chatbot->onExit();

			val.platform->exit();

			delete val.platform;
			delete val.chatbot;
		}

		for (auto& [key, value] : m_Plugins) delete value;
	}

	bool SblaCLI::run()
	{
		std::filesystem::path currentPath = std::filesystem::current_path();
		std::filesystem::path pluginsPath = currentPath / "Plugins";
		std::filesystem::path cfgPath	  = currentPath / "Sibilla.conf";

		// // Load ChatBot and platform
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
				m_Plugins.insert(std::make_pair(info->name, lib));

				if (info->type == "Platform" || info->type == "ALL")
					for (const std::string& val : info->platform) m_Platforms.insert(std::make_pair(val, lib));

				if (info->type == "Chatbot" || info->type == "ALL")
					for (const std::string& val : info->chatbot) m_Chatbots.insert(std::make_pair(val, lib));

				Console::info("Loaded Plugin: {0}, Version: {1}, Type: {2}", info->name, info->version, info->type);
			}
		}

		if (!std::filesystem::exists(cfgPath))
		{
			Console::warn("No config find. Create default config.");

			std::ofstream defaultConfig(cfgPath);
			if (!defaultConfig.is_open())
			{
				Console::error("Create config error.");
				return false;
			}

			defaultConfig << default_config;
			defaultConfig.close();
		}

		// config handle
		if (m_IniParser.LoadFromFile(cfgPath.string().c_str()) != 0)
		{
			Console::warn("Can not load config.");
			return false;
		}

		for (const std::string& section : m_IniParser.GetSections())
		{
			if (section == "dev")
			{
				std::string loggerLevel = m_IniParser.GetValue("log_level", section);

				if (loggerLevel == "fatal")
					Console::setLevel(LoggerLevel::fatal);
				else if (loggerLevel == "error")
					Console::setLevel(LoggerLevel::error);
				else if (loggerLevel == "warn")
					Console::setLevel(LoggerLevel::warn);
				else if (loggerLevel == "info")
					Console::setLevel(LoggerLevel::info);
				else if (loggerLevel == "debug")
					Console::setLevel(LoggerLevel::debug);
				else if (loggerLevel == "trace")
					Console::setLevel(LoggerLevel::trace);
				else
					Console::warn("Unknown log level: {0}, use default level.", loggerLevel);

				continue;
			}

			if (section == "Sibilla")
			{
				// Configuration to Sibilla
				continue;
			}

			if (m_IniParser.GetValue("type", section) != "Chatbot") continue;

			// # Platform
			// plugin = "Same as Platform name"
			// # ... (Platform setting)

			// # Chatbot
			// plugin = "Same as Chatbot name"
			// platform = "Platform Config Name"
			// # ... (Chatbot setting)
			InstanceBox inst;
			std::string platformConfigName = m_IniParser.GetValue("platform", section);
			std::list<std::string> list	   = m_IniParser.GetSections();
			if (std::find(list.begin(), list.end(), platformConfigName) == list.end())
			{
				Console::warn("Can not find platform config \"{0}\" for {1}", platformConfigName, section);
				continue;
			}

			std::string platformPluginName = m_IniParser.GetValue("plugin", platformConfigName);
			std::string chatbotPluginName  = m_IniParser.GetValue("plugin", section);

			if (m_Platforms.find(platformPluginName) == m_Platforms.end())
			{
				Console::warn("Can not find platform \"{0}\" in plugins", platformPluginName);
				continue;
			}

			if (m_Chatbots.find(chatbotPluginName) == m_Chatbots.end())
			{
				Console::warn("Can not find chatbot \"{0}\" in plugins", chatbotPluginName);
				continue;
			}

			inst.platform = m_Platforms[platformPluginName]->createPlatform(platformPluginName, Config(m_IniParser, platformConfigName));
			inst.chatbot  = m_Chatbots[chatbotPluginName]->createChatbot(chatbotPluginName, Config(m_IniParser, section));

			if (inst.platform == nullptr || inst.chatbot == nullptr)
			{
				Console::warn("Create chatbot \"{0}\" in plugins", chatbotPluginName);

				if (inst.platform == nullptr) Console::warn("Error in create {0}", platformPluginName);
				if (inst.chatbot == nullptr) Console::warn("Error in create {0}", chatbotPluginName);

				continue;
			}

			m_Inst.push_back(inst);
			Console::info("Chatbot {0} loaded with Platform {1}.", chatbotPluginName, platformPluginName);
			inst.platform->bindCallBack(std::bind(&IChatbot::onEvent, inst.chatbot, std::placeholders::_1, std::placeholders::_2));

			inst.platform->init();
			inst.chatbot->onInit();
			inst.chatbot->onEnable();
			inst.platform->run();
		}

		while (GlobalInstance::Instance()->isRunning);
		return true;
	}
} // namespace sbla