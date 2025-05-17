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

		template <typename T>
		T value_or(std::string& key, T& val)
		{
			if (m_Config.find(key) == m_Config.end())
				return val;
			else
				return dynamic_cast<T>(m_Config[key]);
		}

	private:
		std::map<std::string, std::string> m_Config;
	};
} // namespace sbla