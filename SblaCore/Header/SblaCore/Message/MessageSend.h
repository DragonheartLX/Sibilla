#pragma once

#include <cstdint>

#include "SblaCore/Message/Message.h"

namespace sbla
{
	struct MessageSend
	{
		int64_t userId	= 0;
		int64_t groupId = 0;
		Message content = {};

		MessageSend()	= default;

		MessageSend(MessageSend& other)
		{
			userId	= other.userId;
			groupId = other.groupId;
			content = other.content;
		}

		void operator=(MessageSend& other)
		{
			userId	= other.userId;
			groupId = other.groupId;
			content = other.content;
		}
	};
} // namespace sbla