#pragma once

#include <cstdint>
#include <string>

#include "SblaCore/DataProtocol/Chatbot.h"

namespace sbla
{
	class EventData;

	enum class EventType
	{
		Meta = 0,
		Message,
		Notice,
		Request
	};

	struct Event
	{
		std::string id			= "";
		int64_t time			= 0;

		EventType type			= EventType::Meta;
		std::string detail_type = "";
		std::string sub_type	= "";

		Self self				= {};

		EventData* data;
	};
} // namespace sbla