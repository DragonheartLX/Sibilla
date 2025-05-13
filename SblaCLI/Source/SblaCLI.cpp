#include "SblaCLI.h"

#include <SblaCore/Config.h>
#include <SblaInterface/Exports.h>
#include <SblaInterface/IChatBot.h>

#include <filesystem>
#include <functional>
#include <utility>

#include "Console.h"
#include "GlobalInstance.h"
#include "Loader.h"

#if SBL_PLATFORM_WINDOWS
	#define PREFIX 0
#else
	#define PREFIX 3
#endif

namespace sbla
{
	SblaCLI::SblaCLI() {}

	SblaCLI::~SblaCLI()
	{
		for (InstanceInfo info : m_Instance)
		{
			info.adaPtr->exit();
			delete info.adaPtr;
			delete info.cbPtr;
		}

		for (auto& [key, value] : m_Ada) delete value;
		for (auto& [key, value] : m_CB) delete value;
	}

	bool SblaCLI::run()
	{
		// Load ChatBot and Adapter
		std::filesystem::path adaPath(std::filesystem::current_path() / "Adapter");
		std::filesystem::path cbPath(std::filesystem::current_path() / "Bot");

		// Adapter
		{
			if (!std::filesystem::exists(adaPath) || !std::filesystem::is_directory(adaPath))
				Console::warn("No Adapter find,");
			else
			{
				for (const auto& entry : std::filesystem::directory_iterator(adaPath))
				{
					Loader* lib = new Loader(adaPath.string(), entry.path().stem().string().substr(PREFIX));

					if (lib->load()) m_Ada.insert(std::pair(entry.path().stem().string().substr(PREFIX), lib));
				}
			};
		}

		{
			if (!std::filesystem::exists(cbPath) || !std::filesystem::is_directory(cbPath))
				Console::warn("No ChatBot find,");
			else
			{
				for (const auto& entry : std::filesystem::directory_iterator(cbPath))
				{
					Loader* lib = new Loader(cbPath.string(), entry.path().stem().string().substr(PREFIX));

					if (lib->load())
					{
						m_CB.insert(std::pair(entry.path().stem().string().substr(PREFIX), lib));
						if (GlobalInstance::getInstance()->config.find(entry.path().stem().string().substr(PREFIX)) !=
							GlobalInstance::getInstance()->config.end())
						{
							// adpaterType
							// adpaterCfgs
							Config cfg = GlobalInstance::getInstance()->config[entry.path().stem().string().substr(PREFIX)];

							if (cfg["adpater"].has_value())
								if (m_Ada.find(cfg["adpater"].value()) != m_Ada.end())
								{
									InstanceInfo ins = {};
									ins.adapter		 = cfg["adpater"].value();
									ins.chatBot		 = entry.path().stem().string().substr(PREFIX);

									InitInfo info	 = {};
									lib->exec(&info);
									ins.cbPtr	= info.chatBot;

									info.config = &GlobalInstance::getInstance()->config[cfg["adpaterCfg"].value()];

									m_Ada[cfg["adpater"].value()]->exec(&info);
									ins.adaPtr = info.adapter;

									info.adapter->bindCallBack(
										std::bind(&IChatBot::msgProcessCallBack, info.chatBot, std::placeholders::_1, std::placeholders::_2));
									info.adapter->run();

									m_Instance.push_back(ins);

									Console::info("ChatBot>\n\tName: {0}\n\tVersion: {1}", info.chatBotInfo.name, info.chatBotInfo.version);
									Console::info("Adapter>\n\tName: {0}\n\tVersion: {1}", info.adapterInfo.name, info.adapterInfo.version);
								}
						}
					}
				}
			};
		}

		while (GlobalInstance::getInstance()->isRunning);
		return true;
	}
} // namespace sbla