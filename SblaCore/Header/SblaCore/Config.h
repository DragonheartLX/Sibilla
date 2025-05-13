#pragma once

#include <map>
#include <optional>
#include <string>

namespace sbla
{
	class Config
	{
	public:
		Config();
		~Config();

		std::optional<std::string> operator[](const std::string& key);

		std::string name;
		std::map<std::string, std::string> content;
	};
} // namespace sbla