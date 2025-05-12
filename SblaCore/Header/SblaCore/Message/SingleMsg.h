#pragma once

#include <string>

namespace sbla
{
	enum class SingleMsgType
	{
		Null,
		Text,
		Image,
		Reply,
		At,
		File,
		Extend
	};

	struct SingleMsg
	{
		SingleMsgType type = SingleMsgType::Null;
		std::string data   = "";
	};
} // namespace sbla