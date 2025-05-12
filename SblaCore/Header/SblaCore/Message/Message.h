#pragma once

#include <vector>

#include "SblaCore/Message/SingleMsg.h"

namespace sbla
{
	enum class MessageType
	{
		Null,
		Private,
		Group
	};

	struct Message
	{
		MessageType type = MessageType::Null;
		std::vector<SingleMsg> content;

		void operator=(Message& other)
		{
			type	= other.type;
			content = other.content;
		}
	};
} // namespace sbla