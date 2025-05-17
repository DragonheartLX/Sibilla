#pragma once

#include <SblaCore/Utils/Singleton.h>
#include <SblaInterface/Plugin.h>
#include <hv/iniparser.h>

#include <map>
#include <string>
#include <vector>

#include "Loader.h"

namespace sbla
{
	struct InstanceBox
	{
		IPlatform* platform = nullptr;
		IChatbot* chatbot	= nullptr;
	};

	class SblaCLI: public Singleton<SblaCLI>
	{
	public:
		~SblaCLI();

		bool run();

	public:
		IniParser m_IniParser;
		std::map<std::string, Loader*> m_Plugins;

		std::map<std::string, Loader*> m_Platforms;
		std::map<std::string, Loader*> m_Chatbots;

		std::vector<InstanceBox> m_Inst;

		friend class Singleton<SblaCLI>;
		SblaCLI();
	};
} // namespace sbla