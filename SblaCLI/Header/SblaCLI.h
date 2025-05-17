#pragma once

#include <SblaCore/Utils/Singleton.h>
#include <hv/iniparser.h>

#include <map>
#include <string>

#include "Loader.h"

namespace sbla
{
	class SblaCLI: public Singleton<SblaCLI>
	{
	public:
		~SblaCLI();

		bool run();

	public:
		IniParser m_IniParser;

		std::map<std::string, Loader*> m_CB;
		std::map<std::string, Loader*> m_Ada;

		friend class Singleton<SblaCLI>;
		SblaCLI();
	};
} // namespace sbla