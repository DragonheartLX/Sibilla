#include "SblaCore/Config.h"

#include <optional>
#include <string>

namespace sbla
{
	Config::Config()
	{
		name = "NULL";
		content.clear();
	}

	Config::~Config() { content.clear(); }

	std::optional<std::string> Config::operator[](const std::string& key)
	{
		if (content.find(key) == content.end()) return std::nullopt;
		return content[key];
	}
} // namespace sbla