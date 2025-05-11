#pragma once

#include <cstdint>

#include "Message.h"

namespace scom
{
	struct MessageRecv
	{
		int64_t time			   = 0;
		int64_t botId			   = 0;
		int32_t messageId		   = 0;
		int64_t senderId		   = 0;
		int64_t groupId			   = 0;
		std::string senderNickname = "";
		Message content			   = {};

		MessageRecv()			   = default;

		MessageRecv(MessageRecv& other)
		{
			time	  = other.time;
			botId	  = other.botId;
			messageId = other.messageId;
			senderId  = other.senderId;
			groupId	  = other.groupId;
			content	  = other.content;
		}

		void operator=(MessageRecv& other)
		{
			time	  = other.time;
			botId	  = other.botId;
			messageId = other.messageId;
			senderId  = other.senderId;
			groupId	  = other.groupId;
			content	  = other.content;
		}
	};
} // namespace scom