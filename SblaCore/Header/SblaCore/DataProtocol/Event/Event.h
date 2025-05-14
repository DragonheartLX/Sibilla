#pragma once

#include <cstdint>
#include <string>

#include "EventData.h"
#include "SblaCore/DataProtocol/Chatbot.h"
#include "SblaCore/Macros.h"

namespace sbla
{
	enum class EventType
	{
		Meta	= BIT(0),
		Message = BIT(1),
		Notice	= BIT(2),
		Request = BIT(3)
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