#include "SblaCore/Config.h"

#include <string>

namespace sbla
{
	Config::Config(IniParser& ini, std::string section)
	{
		std::list<std::string> keys = ini.GetKeys(section);
		for (std::string& key : keys) m_Config[key] = ini.GetValue(key, section);
	}

	Config::~Config() {}
} // namespace sbla