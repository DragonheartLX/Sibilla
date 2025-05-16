#pragma once

#include <SblaCore/Utils/Singleton.h>

#include <map>
#include <string>
#include <vector>

#include "Loader.h"

namespace sbla
{
	struct InstanceInfo
	{
		std::string adapter = "";
		std::string chatBot = "";

		// IChatBot* cbPtr		= nullptr;
		// IAdapter* adaPtr	= nullptr;
	};

	class SblaCLI: public Singleton<SblaCLI>
	{
	public:
		~SblaCLI();

		bool run();

	public:
		std::vector<InstanceInfo> m_Instance;

		std::map<std::string, Loader*> m_CB;
		std::map<std::string, Loader*> m_Ada;

		friend class Singleton<SblaCLI>;
		SblaCLI();
	};
} // namespace sbla