#pragma once

#include <hv/iniparser.h>

#include <map>
#include <string>

namespace sbla
{
	class Config
	{
	public:
		Config(IniParser& ini, std::string section = "");
		~Config();

		std::string& value_or(const std::string& key, std::string&& val)
		{
			if (m_Config.find(key) == m_Config.end())
				return val;
			else
				return m_Config[key];
		}

	private:
		std::map<std::string, std::string> m_Config;
	};
} // namespace sbla